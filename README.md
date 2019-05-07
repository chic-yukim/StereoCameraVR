# CHIC 스테레오 카메라 기반 AR 샘플

## 설정 파일
### DynamicModuleConfiguration
- `XiStream2` 태그
  - `name`: 카메라 이름
  - `width`, `height`: 캡쳐할 이미지 크기

- `pinhole_camera` 태그
  - [CRSF-Modules: pinhole_camera](https://github.com/chic-yukim/crsf/wiki/CRSF-Modules:-pinhole_camera) 참조

- `video_see_through` 태그
  - [CRSF-Modules: video_see_through](https://github.com/chic-yukim/crsf/wiki/CRSF-Modules:-video_see_through) 참조

- `StereoCameraVR` 어플리케이션 태그
  - `camera_calibration`
    - `parameter_filepath`: HMD 기준 VST 카메라의 transform 을 지정하기 위한 파라미터 파일의 경로

### 파라미터 파일
직접 수정할 필요 없음

- `intrinsic`: (3x3 float) Intrinsic parameter of raw image plane of each camera (K)
- `dist_coeffs`: (5x1 float) Distortion coefficients of each camera (d)
- `R` : (3x3 float) Rotation matrix that transforms from the raw to rectified image
- `new_intrinsic`: (3x3 float) Intrinsic parameter of rectified image plane of each camera (K_new)
- `transform_to_hmd`: (4x4 float) Transformation matrix (R, T) from a camera coordinate system to HMD coordinate system
  (i.e., pose of a camera sensor in the HMD coordinate system)
