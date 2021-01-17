// C++ converted library based on the "Arthur Audio Blueprints"
// Source: https://github.com/ArthurBarthur/ArthursAudioBPs

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FreqVisualizer.generated.h"

UCLASS()
class CONVOLUTIONREVERB_API AFreqVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFreqVisualizer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (DisplayName = "StaticMeshComp"))
		class UStaticMeshComponent* m_pStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis", meta = (DisplayName = "Mesh"))
	 class UStaticMesh* m_pMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis", meta = (DisplayName = "FrequenciesToAnalyze"))
		TArray<float> m_FreqToAnalyze;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis", meta = (DisplayName = "SubmixAnalyzer"))
		class ASubmixAnalyzer* m_pSubmixAnalyzer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis", meta = (DisplayName = "AnalyzeBeautifier"))
		class AAnalyzeBeatifier* m_pAnalyzeBeautifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (DisplayName = "EmissiveMultiplier"))
		float m_EmissiveMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (DisplayName = "MinEmissiveStrength"))
		float m_MinEmissiveStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (DisplayName = "RandomColors"))
		bool m_RandomColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (DisplayName = "RValue"))
		FVector2D m_RValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (DisplayName = "GValue"))
		FVector2D m_GValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color", meta = (DisplayName = "BValue"))
		FVector2D m_BValue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals", meta = (DisplayName = "MinSize"))
		float m_MinSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals", meta = (DisplayName = "MeshScaleMultiplier"))
		float m_MeshScaleMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals", meta = (DisplayName = "MeshRowDistanceOffset"))
		float m_MeshRowDistanceOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals", meta = (DisplayName = "VizMeshes"))
		TArray<class UStaticMeshComponent*> m_pVizMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals", meta = (DisplayName = "BaseMaterial"))
		class UMaterialInterface* m_pBaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals", meta = (DisplayName = "MIDs"))
		TArray<class UMaterialInstanceDynamic*> m_pMIDs;

private:	
	TArray<float> m_LatestSmoothValues;

	float m_DeltaTime;

	FTimerHandle m_DelayHandle;

	//Helper functions
	void Init();
	void SpawnMeshes();
	void InitDynMaterials();
	void ScaleMeshes();
	void UpdateEmissive();
	FLinearColor MakeRandColor();
	void ConstructPreVizMesh();
};
