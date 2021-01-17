// C++ converted library based on the "Arthur Audio Blueprints"
// Source: https://github.com/ArthurBarthur/ArthursAudioBPs

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnalyzeBeatifier.generated.h"

UCLASS()
class CONVOLUTIONREVERB_API AAnalyzeBeatifier : public AActor
{
	GENERATED_BODY()
	
public:	
	AAnalyzeBeatifier();

	virtual void Tick(float DeltaTime) override;

	
	const TArray<float>& GetFinalOutputVals(){return m_FinalOutputValues;};

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	//Analysis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis", meta = (DisplayName = "MaxMagnitudes"))
		TArray<float> m_MaxMagnitudes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis", meta = (DisplayName = "FinalOutputValue"))
		TArray<float> m_FinalOutputValues;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis", meta = (DisplayName = "SubmixAnalyzer"))
		class ASubmixAnalyzer* m_pSubmixAnalyzer;

	//Smoothing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smoothing", meta = (DisplayName = "Attack"))
	float m_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smoothing", meta = (DisplayName = "Release"))
	float m_Release;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Smoothing", meta = (DisplayName = "NormalizeFreqIndependently"))
	bool m_NormalizeFreqIndependent;

	//Lower Threshold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LowerThreshold", meta = (DisplayName = "LowerThreshold"))
		float m_LowerTreshold;

	//Sensitivity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preload Sensitivity", meta = (DisplayName = "PreloadFreqMaxRecorded"))
		bool m_PreloadFreqMaxRec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preload Sensitivity", meta = (DisplayName = "MaxMagnitudesPreload"))
		TArray<float> m_MaxMagnitudesPreload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity", meta = (DisplayName = "RegularlyIncreaseSens"))
		bool m_IncreaseSensRegular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity", meta = (DisplayName = "Interval"))
		float m_Interval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensitivity", meta = (DisplayName = "Aggressiveness"))
		float m_Aggressiveness;

private:	
	//UFUNCTION(BlueprintCallable, Category = "Items", meta = (DisplayName = "GetInventory"))

	//Use to handle wether to decreased the maximum magnitudes that got analyzed or not.
	bool m_ShouldDecreaseMaxValues;
	void DecreaseMaxValues();
	void ResetDecreaseMaxValues();
	FTimerHandle m_DecMaxValsTimerHandle;

	float m_DeltaTime;

	//Analysis
	TArray<float> m_FreqToAnalyze;
	TArray<float> m_LatestMagnitudes;
	TArray<float> m_NormalizedValues;
	TArray<float> m_SmoothValues;

	//Lower Threshold
	TArray<float> m_ThresholdValues;

	//Helper functions
	void StoreMaxMagnitudes();
	void NormalizeMagnitudes();
	void Smooth();
	bool m_IsSmoothInit;
	void UseTopForMaxMagnitude();
	void LowerThresholdEye();
};
