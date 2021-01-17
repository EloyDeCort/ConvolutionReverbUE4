// C++ converted library based on the "Arthur Audio Blueprints"
// Source: https://github.com/ArthurBarthur/ArthursAudioBPs

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubmixAnalyzer.generated.h"

UCLASS()
class CONVOLUTIONREVERB_API ASubmixAnalyzer : public AActor
{
	GENERATED_BODY()
	
public:	
	ASubmixAnalyzer();

	virtual void Tick(float DeltaTime) override;

	const TArray<float>& GetFreqToAnalyze(){return m_FreqToAnalyze;};
	const TArray<float>& GetLatestMagnitudes(){return m_LatestMagnitudes;};

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analyzer", meta = (DisplayName = "Submix"))
		class USoundSubmix* m_pSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analyzer", meta = (DisplayName = "Frequencies To Analyze"))
		TArray<float> m_FreqToAnalyze;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analyzer", meta = (DisplayName = "LatestMagnitudes"))
		TArray<float> m_LatestMagnitudes;
private:
};
