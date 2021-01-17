// C++ converted library based on the "Arthur Audio Blueprints"
// Source: https://github.com/ArthurBarthur/ArthursAudioBPs


#include "AnalyzeBeatifier.h"
#include "SubmixAnalyzer.h"
#include "Sound/SoundSubmix.h"
#include "AudioMixerBlueprintLibrary.h"
#include "AudioMixer.h"
#include "Kismet/KismetMathLibrary.h"

AAnalyzeBeatifier::AAnalyzeBeatifier()
	: m_ShouldDecreaseMaxValues{false}
	, m_IsSmoothInit{false}
	, m_Attack{50.f}
	, m_Release{500.f}
	, m_NormalizeFreqIndependent{true}
	, m_LowerTreshold{0.3f}
	, m_IncreaseSensRegular{true}
	, m_Interval{0.25f}
	, m_Aggressiveness{0.1f}
	, m_PreloadFreqMaxRec{false}
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
}

void AAnalyzeBeatifier::BeginPlay()
{
	Super::BeginPlay();
	
	if(m_pSubmixAnalyzer)
	{
		m_FreqToAnalyze = m_pSubmixAnalyzer->GetFreqToAnalyze();
	}

	if(m_PreloadFreqMaxRec)
	{
		//Preloaded magnitudes
		m_MaxMagnitudes = m_MaxMagnitudesPreload;
	}
	else
	{
		//Reset the max magnitudes
		m_MaxMagnitudes.Init(0.00001f, m_FreqToAnalyze.Num());
	}

	m_NormalizedValues.Init(0.00001f, m_MaxMagnitudes.Num());

	SetActorTickEnabled(true);

	if(m_IncreaseSensRegular)
	{
		//Set A timer that will be called every interval
		GetWorldTimerManager().SetTimer(m_DecMaxValsTimerHandle, this, &AAnalyzeBeatifier::ResetDecreaseMaxValues, m_Interval, true);
	}
}

void AAnalyzeBeatifier::OnConstruction(const FTransform& Transform)
{
	if(m_pSubmixAnalyzer)
	{
		m_FreqToAnalyze = m_pSubmixAnalyzer->GetFreqToAnalyze();
	}
}

void AAnalyzeBeatifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_DeltaTime = DeltaTime;

	//Get Latest Analysis
	m_LatestMagnitudes = m_pSubmixAnalyzer->GetLatestMagnitudes();

	//Magnitudes
	StoreMaxMagnitudes();
	NormalizeMagnitudes();

	//Smoothing
	if(!m_IsSmoothInit)
	{
		//Reset the max magnitudes
		m_SmoothValues.Init(0.f, m_NormalizedValues.Num());
		m_IsSmoothInit = true;
	}
	Smooth();

	//Threshold
	LowerThresholdEye();

	if(m_ShouldDecreaseMaxValues)
	{
		DecreaseMaxValues();
	}
}



void AAnalyzeBeatifier::ResetDecreaseMaxValues()
{
	m_ShouldDecreaseMaxValues = true;
}

void AAnalyzeBeatifier::StoreMaxMagnitudes()
{
	for(int i = 0; i < m_LatestMagnitudes.Num(); ++i)
	{
		if(m_LatestMagnitudes[i] > m_MaxMagnitudes[i])
		{
			m_MaxMagnitudes[i] = m_LatestMagnitudes[i];
		}
	}

}

void AAnalyzeBeatifier::NormalizeMagnitudes()
{
	float maxMagnitude{0.f};
	int idx{};
	UKismetMathLibrary::MaxOfFloatArray(m_MaxMagnitudes, idx ,maxMagnitude);
	for(int i = 0; i < m_LatestMagnitudes.Num(); ++i)
	{
		float rangeB{0.f};
		if(m_NormalizeFreqIndependent)
		{
			rangeB = m_MaxMagnitudes[i];
		}
		else
		{
			rangeB = maxMagnitude;
		}

		//Set the clamped value
		m_NormalizedValues[i] = UKismetMathLibrary::MapRangeClamped(m_LatestMagnitudes[i], 0.f, rangeB, 0.f, 1.f );
	}
}

void AAnalyzeBeatifier::Smooth()
{
	//Make sure size valid
	for(int i = 0; i < m_NormalizedValues.Num(); ++i)
	{
		float interpSpeed{0.f};


		//How fast do we have to interpolate + Attack VS Release.
		if(m_NormalizedValues[i] > m_SmoothValues[i])
		{
			interpSpeed = 1.f / (m_Attack *  0.001f);
		}
		else
		{
			interpSpeed = 1.f / (m_Release *  0.001f);
		}

		//Interpolate between the values to smoothing it out (this brings a smoother visualization)
		m_SmoothValues[i] = UKismetMathLibrary::FInterpTo_Constant(m_SmoothValues[i], m_NormalizedValues[i], m_DeltaTime, interpSpeed);
	}
}



void AAnalyzeBeatifier::UseTopForMaxMagnitude()
{
	float topMagnitude{0.f};
	int idx{};
	UKismetMathLibrary::MaxOfFloatArray(m_MaxMagnitudes, idx ,topMagnitude);

	//Set all values to max magnitude
	for(int i = 0; i < m_MaxMagnitudes.Num(); ++i)
	{
		m_MaxMagnitudes[i] = topMagnitude;
	}

}

void AAnalyzeBeatifier::LowerThresholdEye()
{
	TArray<float> temp{};
	temp.Empty();
	
	for(int i = 0; i < m_SmoothValues.Num(); ++i)
	{
		temp.Push(UKismetMathLibrary::MapRangeClamped(m_SmoothValues[i], m_LowerTreshold, 1.f, 0.f, 1.f));
	}

	m_ThresholdValues = temp;
	m_FinalOutputValues = temp;
}


void AAnalyzeBeatifier::DecreaseMaxValues()
{
	for(int i = 0; i < m_MaxMagnitudes.Num(); ++i)
	{
		m_MaxMagnitudes[i] *= UKismetMathLibrary::MapRangeClamped(m_Aggressiveness, 0.f, 1.f, 1.f, 0.f);
		m_MaxMagnitudes[i] = UKismetMathLibrary::FClamp(m_MaxMagnitudes[i], 0.00001f, 9999.f);
	}

	m_ShouldDecreaseMaxValues = false;
}


