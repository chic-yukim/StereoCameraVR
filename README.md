# CHIC 스테레오 카메라 기반 AR 샘플

## 설정 파일
### DynamicModuleConfiguration
- `pinhole_camera` 태그
  - `input_image_memory_object` 태그: Image Memory Object 이름 추가
  - `parameter_filepath`: **파라미터 파일 사용 시** 파라미터 파일의 경로
    - 파라미터 파일 미사용 시에는 파라미터 파일에 필요한 태그들을 `<left>` 태그와 `<right>` 태그에 추가해주면 됩니다.
  - `left`, `right`: 왼쪽/오른쪽 카메라 속성
    - `name`: 카메라 이름
  - `enable_undistortion`: Undistortion 기능 활성화 여부

- `video_see_through` 태그
  - `flip_y`: Vertical flip 활성화 여부
  - `use_valid_vst_depth`: CRSF 의 Object 중에서 VST 카메라에 보여지는 물체에 대해서만 MR 처리
  - `cameras`: 왼쪽/오른쪽 카메라 이름

- `StereoCameraVR` 어플리케이션 태그
  - `camera_calibration`
    - `parameter_filepath`: HMD 기준 VST 카메라의 transform 을 지정하기 위한 파라미터 파일의 경로
