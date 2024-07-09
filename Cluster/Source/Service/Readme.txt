[필요 라이브러리]
	- ssfs
	- hvehicle
	
[디버깅용 로그 출력]
	- Service.h 파일 내부 USE_SERVCIE_DATA_TEST 주석 해제
	- slotServiceDatasChanged() 함수 내부 로그 출력 참조
		[20240705 11:19:52.190] [D] Service::slotServiceDatasChanged : 0 44 8
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Stat" : "2"
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Stat" : "2"
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Value" : "999"
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LatPos.Value" : "23"
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Stat" : "2"
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LongPos.Stat" : "2"
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontRightVehicle.LatPos.Value" : "40"
		[20240705 11:19:52.190] [D]      "SFC.ADAS_Driving_New.Constant.ViewFrontLeftVehicle.LongPos.Value" : "200"

		
[SFC 노드 추가 방버]
	-아래 함수 내부에 type, node 형태로 추가
		void subscribeConstantSignals();
		void subscribeConstantSignals();
		void subscribeTelltaleSignals();
		void subscribeEventSignals();
		void subscribeSoundSignals();
		void subscribeEtcSignals();
	
	
	-추가 방법 예제
		void Service::subscribeConstantSignals() {
			subscribeSignals<Constant>(
				DataType::Constant,
				{
					{Constant::SpeedAnalogStat, {SFC.Speed_Gauge.Constant.SpeedAnalog.Stat}},
					{Constant::SpeedAnalogValue, {SFC.Speed_Gauge.Constant.SpeedAnalog.Value}},
					{Constant::SpeedDigitalStat, {SFC.Speed_Gauge.Constant.SpeedDigital.Stat}},
						....
			
					// SFC 노드 추가 : type, node 형태로 추가
					{Constant::TEST_NODE, {SFC.test.node}},
						....

[SFC 노드 시그널]
	다음 2개 시그널 참고
		void slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue);
		void slotServiceDatasChanged(const int& dataType, const int& signalType, const QHash<QString, QVariant>& signalValues);

	
