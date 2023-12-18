# SFC Application : Test_Application
	- Cluster HMI


개발 환경

앱 구동 화면

화면 구성 레이어 구조
	- display enum

아키텍터 구조
	- mvc 구조에 대한 예시 및 설명

QML
	- property 선언 방법

Handler
	- qml, object, displayType 등록 방법
		QML QString("qrc:/Home/Home.qml") / QML_OBJECT QString("dataModelHome")
	- QML property 와 연동 등록 및 초기값 설정
    	registerProperty(ivis::common::PropertyEnum::CommonDisplay, QString("display"), QVariant(0));

Control
	- 초기값 설정
    	updateDataHandler(ivis::common::PropertyEnum::CommonDisplay, displayType);


Telltale 데이터 업데이트 방법
	- 다이어그램 :
		ControlManager : Control 초기화
		Control : init -> connect -> initCommonData
		Service : singnalServiceDataChanged()
		
