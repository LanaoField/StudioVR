// "WaveVR SDK
// © 2019 HTC Corporation. All Rights Reserved.
//
// Unless otherwise required by copyright law and practice,
// upon the execution of HTC SDK license agreement,
// HTC grants you access to and use of the WaveVR SDK(s).
// You shall fully comply with all of HTC’s SDK license agreement terms and
// conditions signed by you and all SDK and API requirements,
// specifications, and documentation provided by HTC to You."

#pragma once
#include "WaveVRPrivatePCH.h"

#include "HeadMountedDisplayBase.h"
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplay.h"
#include "SceneViewExtension.h"
#include "RendererInterface.h"
#include "Logging/LogMacros.h"

#include "PostProcess/PostProcessHMD.h"
#include "WaveVRRender.h"
#include "WaveVRHMD_FrameData.h"
#include "WaveVRDirectPreviewSettings.h"

#include "ARSystem.h"
#include "ARLightEstimate.h"
#include "WaveAR.h"

class FWaveVRSplash;
class IWaveVRPlugin;
class PoseManagerImp;
class WaveVRDirectPreview;

DECLARE_LOG_CATEGORY_EXTERN(WVRHMD, Log, All)

/**
 * WaveVR Head Mounted Display
 */
class FWaveVRHMD : public FHeadMountedDisplayBase, public IARSystemSupport, public FSceneViewExtensionBase
{
private:
	const bool bUseUnrealDistortion;

	TSharedPtr<FWaveVRSplash> WaveVRSplash;

public:
	/** IXRSystemIdentifier interface */
	virtual FName GetSystemName() const
	{
		LOG_FUNC();
		static FName SystemName(TEXT("WaveVR"));
		return SystemName;
	}

	/** IHeadMountedDisplay interface */
	virtual bool IsHMDConnected() override { LOG_FUNC(); return true;/*return bIsHmdConnected; if in project: LINK */}
	virtual bool IsHMDEnabled() const override;
	virtual bool GetHMDMonitorInfo(MonitorInfo&) override;
	virtual void GetEyeRenderParams_RenderThread(const struct FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const override;
	virtual void SetInterpupillaryDistance(float NewInterpupillaryDistance) override;
	virtual float GetInterpupillaryDistance() const override;
	virtual bool GetHMDDistortionEnabled(EShadingPath ShadingPath) const override;
	virtual float GetPixelDenity() const { LOG_FUNC(); return PixelDensity; }
	virtual void SetPixelDensity(const float NewDensity) override;
	virtual FIntPoint GetIdealRenderTargetSize() const override;
	virtual FIntPoint GetIdealDebugCanvasRenderTargetSize() const {LOG_FUNC(); return IHeadMountedDisplay::GetIdealDebugCanvasRenderTargetSize(); }
	virtual bool IsChromaAbCorrectionEnabled() const override;
	virtual bool HasHiddenAreaMesh() const override { LOG_FUNC(); return false; }
	virtual bool HasVisibleAreaMesh() const override { LOG_FUNC(); return false; }
	virtual void DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) override;
	virtual void UpdateScreenSettings(const class FViewport* InViewport) override { LOG_FUNC(); }
	virtual bool NeedsUpscalePostProcessPass() override { LOG_FUNC(); return false; }
	virtual void RecordAnalytics() override { LOG_FUNC(); FHeadMountedDisplayBase::RecordAnalytics(); }
	virtual bool IsRenderingPaused() const { LOG_FUNC(); return IHeadMountedDisplay::IsRenderingPaused();/*false*/ }
	virtual void SetClippingPlanes(float NCP, float FCP) override;

	/** IHeadMountedDisplay interface not used*/
	virtual void EnableHMD(bool allow = true) override;
	virtual void GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const override;
	virtual bool IsSpectatorScreenActive() const override { LOG_FUNC(); return false; /*return FHeadMountedDisplayBase::IsSpectatorScreenActive();*/ } //UE do not support SpectatorScreen on mobile device now.
	virtual class ISpectatorScreenController* GetSpectatorScreenController() override { LOG_FUNC(); return FHeadMountedDisplayBase::GetSpectatorScreenController(); }
	virtual class ISpectatorScreenController const* GetSpectatorScreenController() const override { LOG_FUNC(); return FHeadMountedDisplayBase::GetSpectatorScreenController(); }
	virtual void CreateHMDPostProcessPass_RenderThread(class FRDGBuilder& GraphBuilder, const class FViewInfo& View, const struct FHMDDistortionInputs& Inputs, struct FScreenPassTexture& OutPass) const override { LOG_FUNC(); IHeadMountedDisplay::CreateHMDPostProcessPass_RenderThread(GraphBuilder, View, Inputs, OutPass); }
	virtual float GetDistortionScalingFactor() const override { LOG_FUNC(); return 0; }
	virtual float GetLensCenterOffset() const override { LOG_FUNC(); return 0; }
	virtual void GetDistortionWarpValues(FVector4& K) const override { LOG_FUNC(); }
	virtual bool GetChromaAbCorrectionValues(FVector4& K) const override { LOG_FUNC(); return false; }
	virtual void DrawHiddenAreaMesh_RenderThread(class FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const override { LOG_FUNC(); }
	virtual void DrawVisibleAreaMesh_RenderThread(class FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const override { LOG_FUNC(); }
	virtual FTexture* GetDistortionTextureLeft() const override { LOG_FUNC(); return NULL; }
	virtual FTexture* GetDistortionTextureRight() const override { LOG_FUNC(); return NULL; }
	virtual FVector2D GetTextureOffsetLeft() const override { LOG_FUNC(); return FVector2D::ZeroVector; }
	virtual FVector2D GetTextureOffsetRight() const override { LOG_FUNC(); return FVector2D::ZeroVector; }
	virtual FVector2D GetTextureScaleLeft() const override { LOG_FUNC(); return FVector2D::ZeroVector; }
	virtual FVector2D GetTextureScaleRight() const override { LOG_FUNC(); return FVector2D::ZeroVector; }
	virtual const float* GetRedDistortionParameters() const override { LOG_FUNC(); return nullptr; }
	virtual const float* GetGreenDistortionParameters() const override { LOG_FUNC(); return nullptr; }
	virtual const float* GetBlueDistortionParameters() const override { LOG_FUNC(); return nullptr; }
	virtual bool DoesAppUseVRFocus() const override { LOG_FUNC(); return IHeadMountedDisplay::DoesAppUseVRFocus(); }
	virtual bool DoesAppHaveVRFocus() const override { LOG_FUNC(); return IHeadMountedDisplay::DoesAppHaveVRFocus(); }
	virtual EHMDWornState::Type GetHMDWornState() override { return EHMDWornState::Unknown; }

        //** FHeadMountedDisplayBase interface */
        virtual bool PopulateAnalyticsAttributes(TArray<struct FAnalyticsEventAttribute>& EventAttributes) override { LOG_FUNC(); return FHeadMountedDisplayBase::PopulateAnalyticsAttributes(EventAttributes);}
        virtual FTexture2DRHIRef GetOverlayLayerTarget_RenderThread(EStereoscopicPass StereoPass, FIntRect& InOutViewport) override {LOG_FUNC(); return nullptr; }
        virtual FTexture2DRHIRef GetSceneLayerTarget_RenderThread(EStereoscopicPass StereoPass, FIntRect& InOutViewport) override {LOG_FUNC(); return nullptr; }

        //** FHeadMountedDisplayBase interface not used*/
        virtual FVector2D GetEyeCenterPoint_RenderThread(EStereoscopicPass Eye) const override { LOG_FUNC(); return FHeadMountedDisplayBase::GetEyeCenterPoint_RenderThread(Eye);}
        virtual FIntRect GetFullFlatEyeRect_RenderThread(FTexture2DRHIRef EyeTexture) const override { LOG_FUNC(); return FHeadMountedDisplayBase::GetFullFlatEyeRect_RenderThread(EyeTexture); }
        virtual void CopyTexture_RenderThread(FRHICommandListImmediate& RHICmdList, FRHITexture2D* SrcTexture, FIntRect SrcRect, FRHITexture2D* DstTexture, FIntRect DstRect, bool bClearBlack, bool bNoAlpha) const {
                LOG_FUNC();
                FHeadMountedDisplayBase::CopyTexture_RenderThread(RHICmdList, SrcTexture, SrcRect, DstTexture, DstRect, bClearBlack, bNoAlpha);
        }

	/** IXRTrackingSystem interface */
	virtual bool DoesSupportLateUpdate() const override;
	virtual bool GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition) override;
	virtual bool GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition) override;
	virtual void SetTrackingOrigin(EHMDTrackingOrigin::Type NewOrigin) override;
	virtual bool GetFloorToEyeTrackingTransform(FTransform& OutFloorToEye) const override { LOG_FUNC(); OutFloorToEye = FTransform::Identity; return false; } //TODO
	virtual FVector GetAudioListenerOffset(int32 DeviceId = HMDDeviceId) const override { LOG_FUNC(); return IXRTrackingSystem::GetAudioListenerOffset(); }
	virtual class TSharedPtr< class IXRCamera, ESPMode::ThreadSafe > GetXRCamera(int32 DeviceId = HMDDeviceId) override { LOG_FUNC(); return FXRTrackingSystemBase::GetXRCamera(DeviceId); }
	virtual class TSharedPtr< class IStereoRendering, ESPMode::ThreadSafe > GetStereoRenderingDevice() override { LOG_FUNC(); return SharedThis(this); }
	virtual IXRInput* GetXRInput() override { LOG_FUNC(); return IXRTrackingSystem::GetXRInput(); }
	//virtual class IXRLoadingScreen* GetLoadingScreen() override; // FXRTrackingSystemBase::GetLoadingScreen() is override final.
	virtual bool IsHeadTrackingAllowed() const override;
	virtual void OnBeginPlay(FWorldContext& InWorldContext) override;
	virtual void OnEndPlay(FWorldContext& InWorldContext) override;
	virtual bool OnStartGameFrame( FWorldContext& WorldContext ) override;
	virtual bool OnEndGameFrame(FWorldContext& WorldContext) override;
	virtual void OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily) override;
	virtual void OnBeginRendering_GameThread() override;
	virtual void OnLateUpdateApplied_RenderThread(const FTransform& NewRelativeTransform) override;
	virtual EHMDTrackingOrigin::Type GetTrackingOrigin() const override;
	virtual void ResetOrientationAndPosition(float yaw = 0.f) override;
	virtual void ResetOrientation(float Yaw = 0.f) override;
	virtual void ResetPosition() override;
	virtual void SetBaseRotation(const FRotator& BaseRot) override;
	virtual FRotator GetBaseRotation() const override;
	virtual void SetBaseOrientation(const FQuat& BaseOrient) override;
	virtual FQuat GetBaseOrientation() const override;
	virtual bool DoesSupportPositionalTracking() const override;
	virtual bool HasValidTrackingPosition() override;
	virtual bool EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type = EXRTrackedDeviceType::Any) override;

	/** IXRTrackingSystem interface not used*/
	virtual FTransform GetTrackingToWorldTransform() const override { LOG_FUNC(); return FXRTrackingSystemBase::GetTrackingToWorldTransform(); }
	virtual void UpdateTrackingToWorldTransform(const FTransform& TrackingToWorldOverride) override { LOG_FUNC(); return FXRTrackingSystemBase::UpdateTrackingToWorldTransform(TrackingToWorldOverride); }
	virtual FString GetVersionString() const override { LOG_FUNC(); return FString(TEXT("GenericHMD")); }
	virtual uint32 CountTrackedDevices(EXRTrackedDeviceType Type = EXRTrackedDeviceType::Any) override { LOG_FUNC(); return FXRTrackingSystemBase::CountTrackedDevices(Type); }
	virtual bool IsTracking(int32 DeviceId) override { LOG_FUNC(); return FXRTrackingSystemBase::IsTracking(DeviceId); }
	virtual void RebaseObjectOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation) const override { LOG_FUNC(); }
	virtual bool GetTrackingSensorProperties(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition, FXRSensorProperties& OutSensorProperties) override { LOG_FUNC(); return false; }
	virtual void SetBasePosition(const FVector& BasePosition) override { LOG_FUNC(); } //Do not use
	virtual FVector GetBasePosition() const override { return FVector::ZeroVector; } //Do not use
	virtual void CalibrateExternalTrackingSource(const FTransform& ExternalTrackingTransform) override { LOG_FUNC(); FXRTrackingSystemBase::CalibrateExternalTrackingSource(ExternalTrackingTransform); } //Do not use now, we should reconsider GetBaseOrientation/SetBaseOrientation/GetBasePosition/SetBasePosition if use it.
	virtual void UpdateExternalTrackingPosition(const FTransform& ExternalTrackingTransform) override { LOG_FUNC(); FXRTrackingSystemBase::UpdateExternalTrackingPosition(ExternalTrackingTransform); } //Do not use now, we should reconsider GetBaseOrientation/SetBaseOrientation/GetBasePosition/SetBasePosition if use it.
	virtual class IHeadMountedDisplay* GetHMDDevice() override { return this; }
	virtual float GetWorldToMetersScale() const override;
	virtual bool IsHeadTrackingEnforced() const { LOG_FUNC(); return FHeadMountedDisplayBase::IsHeadTrackingEnforced(); }
	virtual void SetHeadTrackingEnforced(bool bEnabled) { LOG_FUNC(); FHeadMountedDisplayBase::SetHeadTrackingEnforced(bEnabled); }

	/** FXRTrackingSystemBase interface not used*/
	virtual class IXRLoadingScreen* CreateLoadingScreen() override { LOG_FUNC(); return FXRTrackingSystemBase::CreateLoadingScreen(); } //FXRTrackingSystemBase::GetLoadingScreen() will call.

public:
	/** IStereoRendering interface */
	virtual bool IsStereoEnabled() const override;
	virtual bool IsStereoEnabledOnNextFrame() const override { LOG_FUNC(); return IsStereoEnabled(); }
	virtual bool EnableStereo(bool stereo = true) override;
	virtual int32 GetDesiredNumberOfViews(bool bStereoRequested) const override { LOG_FUNC(); return IStereoRendering::GetDesiredNumberOfViews(bStereoRequested); }
	virtual EStereoscopicPass GetViewPassForIndex(bool bStereoRequested, uint32 ViewIndex) const override { LOG_FUNC(); return IStereoRendering::GetViewPassForIndex(bStereoRequested, ViewIndex);}
	virtual uint32 GetViewIndexForPass(EStereoscopicPass StereoPassType) const override { LOG_FUNC(); return IStereoRendering::GetViewIndexForPass(StereoPassType); }
	virtual bool DeviceIsStereoEyePass(EStereoscopicPass Pass) override { LOG_FUNC(); return IStereoRendering::DeviceIsStereoEyePass(Pass); }
	virtual bool DeviceIsStereoEyeView(const FSceneView& View) override { LOG_FUNC(); return IStereoRendering::DeviceIsStereoEyeView(View); }
	virtual bool DeviceIsAPrimaryPass(EStereoscopicPass Pass) override { LOG_FUNC(); return IStereoRendering::DeviceIsAPrimaryPass(Pass); }
	virtual bool DeviceIsAPrimaryView(const FSceneView& View) override { LOG_FUNC(); return IStereoRendering::DeviceIsAPrimaryView(View); }
	virtual bool DeviceIsASecondaryPass(EStereoscopicPass Pass) override { LOG_FUNC(); return IStereoRendering::DeviceIsASecondaryPass(Pass); }
	virtual bool DeviceIsASecondaryView(const FSceneView& View) override { LOG_FUNC(); return IStereoRendering::DeviceIsASecondaryView(View); }
	virtual bool DeviceIsAnAdditionalPass(EStereoscopicPass Pass) override { LOG_FUNC(); return IStereoRendering::DeviceIsAnAdditionalPass(Pass); }
	virtual bool DeviceIsAnAdditionalView(const FSceneView& View) override { LOG_FUNC(); return IStereoRendering::DeviceIsAnAdditionalView(View); }
	virtual void AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const override;
	virtual void SetFinalViewRect(const enum EStereoscopicPass StereoPass, const FIntRect& FinalViewRect) override;
	virtual FVector2D GetTextSafeRegionBounds() const override { LOG_FUNC(); return FVector2D(0.75f, 0.75f); }
	virtual void CalculateStereoViewOffset(const EStereoscopicPass StereoPassType, FRotator& ViewRotation, const float MetersToWorld, FVector& ViewLocation) override;
	virtual FMatrix GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType) const override;
	virtual void InitCanvasFromView(FSceneView* InView, UCanvas* Canvas) override;
	virtual void RenderTexture_RenderThread(class FRHICommandListImmediate& RHICmdList, class FRHITexture2D* BackBuffer, class FRHITexture2D* SrcTexture, FVector2D WindowSize) const override { LOG_FUNC(); }
	virtual IStereoLayers* GetStereoLayers() override { LOG_FUNC(); return FHeadMountedDisplayBase::GetStereoLayers(); }
	virtual IStereoRenderTargetManager* GetRenderTargetManager() override { LOG_FUNC(); return &mRender; }
	virtual void StartFinalPostprocessSettings(struct FPostProcessSettings* StartPostProcessingSettings, const enum EStereoscopicPass StereoPassType) override {
		LOG_FUNC();
		return IStereoRendering::StartFinalPostprocessSettings(StartPostProcessingSettings, StereoPassType);
	}
	virtual bool OverrideFinalPostprocessSettings(struct FPostProcessSettings* OverridePostProcessingSettings, const enum EStereoscopicPass StereoPassType, float& BlendWeight) override {
		LOG_FUNC();
		return IStereoRendering::OverrideFinalPostprocessSettings(OverridePostProcessingSettings, StereoPassType, BlendWeight);
	}
	virtual void EndFinalPostprocessSettings(struct FPostProcessSettings* FinalPostProcessingSettings, const enum EStereoscopicPass StereoPassType) override {
		LOG_FUNC();
		return IStereoRendering::EndFinalPostprocessSettings(FinalPostProcessingSettings, StereoPassType);
	}

	/** IStereoRendering interface not used */

public:
	/** ISceneViewExtension interface */
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override { LOG_FUNC(); }
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override { LOG_FUNC(); }
	virtual void SetupViewPoint(APlayerController* Player, FMinimalViewInfo& InViewInfo) override { LOG_FUNC(); }
	virtual void SetupViewProjectionMatrix(FSceneViewProjectionData& InOutProjectionData) override { LOG_FUNC(); }
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override { LOG_FUNC(); }
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override { LOG_FUNC(); }
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override { LOG_FUNC(); }
	virtual void PostRenderBasePass_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override { LOG_FUNC(); }
	virtual void PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;
	virtual void PostRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override { LOG_FUNC(); }
	virtual int32 GetPriority() const override { LOG_FUNC(); return 0; }
	virtual bool IsActiveThisFrame(class FViewport* InViewport) const override { LOG_FUNC(); return true; }

protected:
	// IARSystemSupport
	virtual void OnARSystemInitialized() override;
	virtual EARTrackingQuality OnGetTrackingQuality() const override;
	virtual EARTrackingQualityReason OnGetTrackingQualityReason() const override;
	virtual void OnStartARSession(UARSessionConfig* SessionConfig) override;
	virtual void OnPauseARSession() override;
	virtual void OnStopARSession() override;
	virtual FARSessionStatus OnGetARSessionStatus() const override;
	virtual bool IsARAvailable() const override;
	virtual void OnSetAlignmentTransform(const FTransform& InAlignmentTransform) override;
	virtual TArray<FARTraceResult> OnLineTraceTrackedObjects(const FVector2D ScreenCoord, EARLineTraceChannels TraceChannels) override;
	virtual TArray<FARTraceResult> OnLineTraceTrackedObjects(const FVector Start, const FVector End, EARLineTraceChannels TraceChannels) override;
	virtual TArray<UARTrackedGeometry*> OnGetAllTrackedGeometries() const override;
	virtual TArray<UARPin*> OnGetAllPins() const override;
	virtual bool OnIsTrackingTypeSupported(EARSessionType SessionType) const override;
	virtual UARLightEstimate* OnGetCurrentLightEstimate() const override;

	virtual UARPin* OnPinComponent(USceneComponent* ComponentToPin, const FTransform& PinToWorldTransform, UARTrackedGeometry* TrackedGeometry = nullptr, const FName DebugName = NAME_None) override;
	virtual void OnRemovePin(UARPin* PinToRemove) override;
	virtual UARTextureCameraImage* OnGetCameraImage() override { LOG_FUNC(); return nullptr; }
	virtual UARTextureCameraDepth* OnGetCameraDepth() override { LOG_FUNC(); return nullptr; }
	virtual bool OnAddManualEnvironmentCaptureProbe(FVector Location, FVector Extent) { LOG_FUNC(); return false; }
	virtual TSharedPtr<FARGetCandidateObjectAsyncTask, ESPMode::ThreadSafe> OnGetCandidateObject(FVector Location, FVector Extent) const { LOG_FUNC(); return TSharedPtr<FARGetCandidateObjectAsyncTask, ESPMode::ThreadSafe>(); }
	virtual TSharedPtr<FARSaveWorldAsyncTask, ESPMode::ThreadSafe> OnSaveWorld() const { LOG_FUNC(); return TSharedPtr<FARSaveWorldAsyncTask, ESPMode::ThreadSafe>(); }
	virtual EARWorldMappingState OnGetWorldMappingStatus() const override;
	virtual TArray<FARVideoFormat> OnGetSupportedVideoFormats(EARSessionType SessionType) const override { LOG_FUNC(); return TArray<FARVideoFormat>(); }
	virtual TArray<FVector> OnGetPointCloud() const override;
	virtual bool OnAddRuntimeCandidateImage(UARSessionConfig* SessionConfig, UTexture2D* CandidateTexture, FString FriendlyName, float PhysicalWidth) override;


public:
	/** FARSystemBase interface */
	void* GetARSessionRawPointer() override;
	void* GetGameThreadARFrameRawPointer() override;
protected:
	//~ FGCObject
	virtual void AddReferencedObjects(FReferenceCollector& Collector) /*override*/;

public:
	FrameDataPtr FrameData;
	FrameDataPtr FrameDataRT;
	FrameDataPtr OldFrameData;
	FrameDataPtr OldFrameDataRT;

private:
	void NextFrameData();

public:
	static void SetARSystem(TSharedPtr<IARSystemSupport, ESPMode::ThreadSafe> InArSystem) { ArSystem = InArSystem; }
	static TSharedPtr<IARSystemSupport, ESPMode::ThreadSafe> GetARSystem() { return ArSystem; }

private:
	static TSharedPtr<IARSystemSupport, ESPMode::ThreadSafe> ArSystem;

public:
	bool IsStereoEnabledInternal() const;
	FVector GetCompensationOffset() const;
	bool IsFocusCapturedBySystem() const;
	bool IsDeviceConnected(WVR_DeviceType device) const;
	void SetInAppRecenter(bool enabled = false);
	bool IsInAppRecenter();
	bool IsDirectPreview();
	void SetInputRequest(WVR_DeviceType device, const WVR_InputAttribute_t* inputAttributes, uint32_t size);
	void SetInputRequestAll();
	void UpdateInputMappingTable(WVR_DeviceType device);
	bool GetInputMappingPair(WVR_DeviceType device, WVR_InputId &destination);
	bool IsButtonAvailable(WVR_DeviceType device, WVR_InputId button);
	void SceneStatusInfo(bool IsFirstGameFrame) const;
	bool IsRenderFoveationSupport();
	bool IsRenderFoveationEnabled();
	void SetFoveationMode(WVR_FoveationMode Mode);
	void SetFoveationParams(EStereoscopicPass Eye, WVR_RenderFoveationParams_t& FoveatParams);
	void GetFoveationParams(EStereoscopicPass Eye, WVR_RenderFoveationParams_t& FoveatParams);
	bool IsSplashShowing();
	TSharedPtr<FWaveVRSplash> GetSplashScreen() { LOG_FUNC(); return WaveVRSplash; }
	void SetAdaptiveQualityState(bool enabled, uint32_t strategyFlags);
	void EnableLateUpdate(bool enable, bool doUpdateInGT, float predictTimeInGT);

public:
	bool IsRenderInitialized();
	void SimulateCPULoading(unsigned int gameThreadLoading, unsigned int renderThreadLoading);

private:
	// ---------------------------------- HMD ----------------------------------
	static const int InputAttributes_HMD_Count = 2;
	WVR_InputAttribute InputAttributes_HMD[InputAttributes_HMD_Count] = {
		{WVR_InputId_Alias1_Back, WVR_InputType_Button, WVR_AnalogType_None},
		{WVR_InputId_Alias1_Enter, WVR_InputType_Button, WVR_AnalogType_None},
	};
	bool usableButtons_hmd[(int)WVR_InputId_Max] = { false };
	WVR_InputMappingPair_t inputTableHMD[(int)WVR_InputId_Max];
	uint32_t inputTableHMDSize;


	// ---------------------------------- Controller ----------------------------------
	static const int InputAttributes_Controller_Count = 11;
	WVR_InputAttribute InputAttributes_Controller[InputAttributes_Controller_Count] = {
		{WVR_InputId_Alias1_Menu, WVR_InputType_Button, WVR_AnalogType_None},
		{WVR_InputId_Alias1_Grip, WVR_InputType_Button, WVR_AnalogType_None},
		{WVR_InputId_Alias1_DPad_Left, WVR_InputType_Button, WVR_AnalogType_None},
		{WVR_InputId_Alias1_DPad_Up, WVR_InputType_Button, WVR_AnalogType_None},
		{WVR_InputId_Alias1_DPad_Right, WVR_InputType_Button, WVR_AnalogType_None},		// 5
		{WVR_InputId_Alias1_DPad_Down, WVR_InputType_Button, WVR_AnalogType_None},
		{WVR_InputId_Alias1_A, WVR_InputType_Button, WVR_AnalogType_None},
		{WVR_InputId_Alias1_B, WVR_InputType_Button, WVR_AnalogType_None},
		//{WVR_InputId_Alias1_Bumper, WVR_InputType_Button, WVR_AnalogType_None},	// NOT request Bumper by default.

		{WVR_InputId_Alias1_Touchpad, WVR_InputType_Button | WVR_InputType_Touch | WVR_InputType_Analog, WVR_AnalogType_2D},
		{WVR_InputId_Alias1_Thumbstick, WVR_InputType_Button | WVR_InputType_Touch | WVR_InputType_Analog, WVR_AnalogType_2D},	// 10

		{WVR_InputId_Alias1_Trigger, WVR_InputType_Button | WVR_InputType_Touch | WVR_InputType_Analog, WVR_AnalogType_1D},
	};
	bool usableButtons_right[(int)WVR_InputId_Max] = { false };
	bool usableButtons_left[(int)WVR_InputId_Max] = { false };
	WVR_InputMappingPair_t inputTableRight[(int)WVR_InputId_Max];
	WVR_InputMappingPair_t inputTableLeft[(int)WVR_InputId_Max];
	uint32_t inputTableRightSize;
	uint32_t inputTableLeftSize;

public:	// Gesture
	WVR_EventType GetGestureEvent() { return gestureEventCurr; }
	void SetGestureEvent(WVR_EventType event) { gestureEventCurr = event; }

	WVR_Result StartHandGesture() {
		WVR_Result result = WVR()->StartHandGesture();
		handGestureEnabled = result == WVR_Result::WVR_Success ? true : false;

		return result;
	}
	void StopHandGesture() {
		if (handGestureEnabled)
		{
			WVR()->StopHandGesture();
			handGestureEnabled = false;
		}
	}
	WVR_Result StartHandTracking() {
		WVR_Result result = WVR()->StartHandTracking();
		handTrackingEnabled = result == WVR_Result::WVR_Success ? true : false;

		return result;
	}
	void StopHandTracking() {
		if (handTrackingEnabled)
		{
			WVR()->StopHandTracking();
			handTrackingEnabled = false;
		}
	}
private:
	WVR_EventType gestureEventCurr;
	bool handGestureEnabled;
	bool handTrackingEnabled;

	// DistortionCorrection
private:
	/** Helper method to generate index buffer for manual distortion rendering */
	void GenerateDistortionCorrectionIndexBuffer();
	/** Helper method to generate vertex buffer for manual distortion rendering */
	void GenerateDistortionCorrectionVertexBuffer(EStereoscopicPass Eye);
	/** Generates Distortion Correction Points*/
	void SetNumOfDistortionPoints(int32 XPoints, int32 YPoints);

private:
	void ApplicationWillEnterBackgroundDelegate();
	void ApplicationWillDeactivateDelegate();
	void ApplicationHasReactivatedDelegate();
	void ApplicationHasEnteredForegroundDelegate();
	void OnFEngineLoopInitComplete();
	void OnPreLoadMap(const FString&);
	void OnPostLoadMap(UWorld*);

	void setDeviceSupportedMaxFPS();
	bool bResumed;
	uint64_t supportedFeatures;

public:

	FWaveVRHMD(const FAutoRegister& AutoRegister);
	virtual ~FWaveVRHMD();

	// distortion mesh
	uint32 DistortionPointsX;
	uint32 DistortionPointsY;
	uint32 NumVerts;
	uint32 NumTris;
	uint32 NumIndices;
	FHMDViewMesh HiddenAreaMeshes[2];
	FHMDViewMesh VisibleAreaMeshes[2];

	uint16* DistortionMeshIndices;
	FDistortionVertex* DistortionMeshVerticesLeftEye;
	FDistortionVertex* DistortionMeshVerticesRightEye;
	//WaveVRDistortion* mWaveVRDistort;
	FSceneViewFamily* mViewFamily;
	uint64_t GetSupportedFeatures() { return supportedFeatures; }

private:
	//Returns true if initialization successfully, false if not.
	bool Startup();
	void OnPause();
	void OnResume();
	// Shuts down WaveVR
	void Shutdown();

	void pollEvent();
	void processVREvent(WVR_Event_t vrEvent);
	void ResetProjectionMats();
	void checkSystemFocus();

private:
	bool bIsHmdConnected;
	bool bIsRightDeviceConnected;
	bool bIsLeftDeviceConnected;

	bool bHmdEnabled;
	bool bStereoEnabled;
	float CurrentIPD;
	float NearClippingPlane;
	float FarClippingPlane;
	bool bNeedResetProjectionMatrix;

	FWaveVRRender mRender;
	PoseManagerImp* PoseMngr;
	FWaveAR* mWaveAR;

	/** Consider Player's orientation tracking, both HMD's orientation and players controll motion will be accumulated */
	FRotator	DeltaControlRotation;    // same as DeltaControlOrientation but as rotator
	FQuat	DeltaControlOrientation; // same as DeltaControlRotation but as quat

	FMatrix LeftProjection;
	FMatrix RightProjection;
	FMatrix CenterProjection;

	// Game Position / Teleport
	FQuat BaseOrientation;
	FVector CompensationOffset;
	float Current_Yaw;

	float PixelDensity;
	bool bFocusCapturedBySystem;
	bool bIsInAppRecenter;
	bool bSIM_Available;
	bool FirstGameFrame;
	bool bAdaptiveQuality;
	uint32_t AdaptiveQualityStrategyFlags;
	bool AppliedAdaptiveQualityProjectSettings;
	static WaveVRDirectPreview * DirectPreview;

	struct LateUpdateConfig {
		LateUpdateConfig() : bEnabled(true), bDoUpdateInGT(false), predictTimeInGT(0) {}
		bool bEnabled;
		bool bDoUpdateInGT;  // Only work if LateUpdate is enabled
		float predictTimeInGT;  // Only work if LateUpdate and DoUpdateInGT is enabled
	};

	LateUpdateConfig lateUpdateConfig;

	friend class FWaveVRPlugin;
	friend class FWaveVRRender;

	unsigned int SimulatedLoadingGameThread;
	unsigned int SimulatedLoadingRenderThread;

protected:
	static FWaveVRHMD * Instance;

public:
	static inline FWaveVRHMD * GetInstance() {
		if (Instance != nullptr)
			return Instance;
		else
			return nullptr;
	}
};
