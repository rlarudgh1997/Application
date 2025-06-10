# Appendix. Implementation Guide for Application Feedback Required Node Address

## SFC.Extension.Settings.*

### Purpose

SFC 와 Application 연동 구현 기능에 대한 설명

Settings SFC 에서 필요로 하는 Signal Value 이다.

USM 연동을 필요로 한다.

### Sequence Diagram

![image-20210125104430181](../../extension/Release_Document_Contents/CV/Guide.Settings.assets/image-20210125104430181.png)

## SFC.Extension.Memory.*

### Purpose

SFC 와 Application 연동 구현 기능에 대한 설명

사용자별 상태값들을 저장하기 위한 기능이다.

OdoMeter 와 같은 보안성 정보는 별도의 사양에 따라 처리한다.

### Sequence Diagram

![image-20210125104849179](../../extension/Release_Document_Contents/CV/Guide.Settings.assets/image-20210125104849179.png)

## SFC.Extension.Param.*

### Purpose

SFC 와 Application 연동 구현 기능에 대한 설명

튜닝값을 저장하기 위한 기능이다.

초기값들을 저장하고 Engineering Mode App 에서 수정한 값들을 반영하는 로직이다.

### Sequence Diagram

![image-20210125104943345](../../extension/Release_Document_Contents/CV/Guide.Settings.assets/image-20210125104943345.png)
