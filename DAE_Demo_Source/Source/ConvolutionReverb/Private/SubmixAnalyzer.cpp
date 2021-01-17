// C++ converted library based on the "Arthur Audio Blueprints"
// Source: https://github.com/ArthurBarthur/ArthursAudioBPs

#include "SubmixAnalyzer.h"
#include "Sound/SoundSubmix.h"
#include "AudioMixerBlueprintLibrary.h"
#include "AudioMixer.h"

ASubmixAnalyzer::ASubmixAnalyzer()
	: m_pSoundMix{nullptr}
	, m_FreqToAnalyze{50.f,100.f,200.f,400.f,800.f,1600.f,2400.f,3200.f,5000.f, 8000.f, 15000.f, 18500.f}
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASubmixAnalyzer::BeginPlay()
{
	Super::BeginPlay();


	//Set Standard magnitude values
	m_LatestMagnitudes.Init(0.01f, m_FreqToAnalyze.Num());

	//Start analyzing of the output of the sound mix.
	UAudioMixerBlueprintLibrary::StartAnalyzingOutput(GetWorld(), m_pSoundMix);
	
}


void ASubmixAnalyzer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get the latest magnitudes
	UAudioMixerBlueprintLibrary::GetMagnitudeForFrequencies(GetWorld(), m_FreqToAnalyze, m_LatestMagnitudes, m_pSoundMix);
}

