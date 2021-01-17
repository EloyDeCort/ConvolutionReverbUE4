// C++ converted library based on the "Arthur Audio Blueprints"
// Source: https://github.com/ArthurBarthur/ArthursAudioBPs

#include "FreqVisualizer.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AnalyzeBeatifier.h"
#include "SubmixAnalyzer.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFreqVisualizer::AFreqVisualizer()
	: m_EmissiveMultiplier{30.f}
	, m_MinEmissiveStrength{0.01f}
	, m_RValue{0.f, 0.5f}
	, m_GValue{0.f, 0.0f}
	, m_BValue{0.f, 0.5f}
	, m_MinSize{1.f}
	, m_MeshScaleMultiplier{10.f}
	, m_MeshRowDistanceOffset{130.f}
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	m_pStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");

	SetRootComponent(m_pStaticMeshComponent);
	/*UBlueprint* Blueprint = Cast<UBlueprint>(GetClass()->ClassGeneratedBy);
	if (Blueprint)
	{
		Blueprint->bRunConstructionScriptOnDrag = false;
	}*/
}

// Called when the game starts or when spawned
void AFreqVisualizer::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(m_DelayHandle, this, &AFreqVisualizer::Init,0.5f, false);

	if(m_pSubmixAnalyzer)
	{
		m_FreqToAnalyze = m_pSubmixAnalyzer->GetFreqToAnalyze();

		SpawnMeshes();
		InitDynMaterials();
		ConstructPreVizMesh();
	}
}

void AFreqVisualizer::OnConstruction(const FTransform& Transform)
{
	
}

void AFreqVisualizer::Init()
{
	SetActorTickEnabled(true);
}

void AFreqVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_DeltaTime = DeltaTime;

	//Get Beautified values
	m_LatestSmoothValues = m_pAnalyzeBeautifier->GetFinalOutputVals();

	ScaleMeshes();

	UpdateEmissive();
}

void AFreqVisualizer::SpawnMeshes()
{

	//Reset
	for(int i = 0; i < m_pVizMeshes.Num(); ++i)
	{
		m_pVizMeshes[i]->DestroyComponent();
	}
	m_pVizMeshes.Empty();

	m_pVizMeshes.SetNum(m_FreqToAnalyze.Num());

	for(int i = 0; i < m_FreqToAnalyze.Num(); ++i)
	{
		UStaticMeshComponent* comp = NewObject<UStaticMeshComponent>(this);
		comp->AttachToComponent(m_pStaticMeshComponent, FAttachmentTransformRules::KeepRelativeTransform); 
		comp->RegisterComponent(); 

		comp->SetRelativeLocation(FVector(0.f, i * m_MeshRowDistanceOffset, 0.f));
		comp->SetStaticMesh(m_pMesh); 

		m_pVizMeshes[i] = comp;

	}
}

void AFreqVisualizer::InitDynMaterials()
{
	m_pMIDs.SetNum(m_pVizMeshes.Num());
	for(int i = 0; i < m_pVizMeshes.Num(); ++i)
	{
		//m_pBaseMaterial
		m_pMIDs[i] = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), m_pBaseMaterial);

		m_pVizMeshes[i]->SetMaterial(0,m_pMIDs[i]);

		if(m_RandomColors)
		{
			m_pMIDs[i]->SetVectorParameterValue("BaseColor", MakeRandColor());
		}
	}
}

void AFreqVisualizer::ScaleMeshes()
{
	for(int i = 0; i < m_pVizMeshes.Num(); ++i)
	{
		FVector scale{1.f,1.f, (m_LatestSmoothValues[i] * m_MeshScaleMultiplier) + m_MinSize};
		m_pVizMeshes[i]->SetRelativeScale3D(scale);
	}
}

void AFreqVisualizer::UpdateEmissive()
{
	for(int i = 0; i < m_pMIDs.Num(); ++i)
	{
		m_pMIDs[i]->SetScalarParameterValue("EmissiveMultiplier",	(m_LatestSmoothValues[i] + m_MinEmissiveStrength) * m_EmissiveMultiplier);
	}
}

FLinearColor AFreqVisualizer::MakeRandColor()
{
	FLinearColor result = FLinearColor::MakeRandomColor();
	result.A = 0.f;

	return result;
}

void AFreqVisualizer::ConstructPreVizMesh()
{
	for(int i = 0; i < m_pVizMeshes.Num(); ++i)
	{
		float rand = 0.f;
		if(UKismetMathLibrary::RandomBoolWithWeight(0.7f))
		{
			rand = UKismetMathLibrary::RandomFloatInRange(0.f, 0.5f);
		}
		else
		{
			rand = UKismetMathLibrary::RandomFloatInRange(0.5f, 1.f);
		}
		
		float val = (rand * m_MeshScaleMultiplier) + m_MinSize;
		FVector scale{1.f,1.f, val};
		m_pVizMeshes[i]->SetRelativeScale3D(scale);

		m_pMIDs[i]->SetScalarParameterValue("EmissiveMultiplier", val * m_EmissiveMultiplier);
	}
}

