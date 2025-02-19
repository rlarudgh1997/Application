"""
SignalCollection.py

나중에 TCName 단위로 case를 생성하게 되면 그때 이 class 를 사용하여 관리하고 other 를 생성하는 방식을 구현해야 함.
"""

from .SignalCollection import SignalCollection
import itertools
import copy


class SignalCollectionManager:
    def __init__(self):
        # 여러 SignalCollection 객체를 보관할 리스트
        self.collections = []

    def add_collection(self, collection):
        """SignalCollection 객체를 추가합니다."""
        if isinstance(collection, SignalCollection):
            self.collections.append(collection)
        else:
            raise TypeError("Only SignalCollection objects can be added.")

    def remove_collection(self, index):
        """인덱스를 기반으로 특정 SignalCollection 객체를 제거합니다."""
        if 0 <= index < len(self.collections):
            self.collections.pop(index)
        else:
            raise IndexError("Invalid index for removing collection.")

    def get_collection(self, index):
        """인덱스를 기반으로 특정 SignalCollection 객체를 반환합니다."""
        if 0 <= index < len(self.collections):
            return self.collections[index]
        else:
            raise IndexError("Invalid index for accessing collection.")

    def display_all_collections(self):
        """모든 SignalCollection 객체의 정보를 출력합니다."""
        for idx, collection in enumerate(self.collections):
            print(f"SignalCollection {idx}:")
            collection.display_signals()  # 각 collection의 display_signals 메서드를 호출
            print("-" * 40)

    def find_collection_by_signal_name(self, signal_name):
        """특정 Signal 이름을 가진 SignalCollection을 검색합니다."""
        results = []
        for collection in self.collections:
            for signal in collection.signals:
                if signal.InputSignalName == signal_name:
                    results.append(collection)
                    break
        return results


# 사용 예제
# SignalCollection을 추가하고 관리하는 객체 생성
manager = SignalCollectionManager()

# SignalCollection 객체들을 생성하고 추가합니다.
signal_collection_1 = SignalCollection(signal_objects)  # 예시 데이터
signal_collection_2 = SignalCollection(signal_objects)  # 예시 데이터

# SignalCollectionManager에 추가
manager.add_collection(signal_collection_1)
manager.add_collection(signal_collection_2)

# 모든 SignalCollection의 내용을 출력
manager.display_all_collections()

# 특정 이름을 가진 Signal을 포함하는 Collection을 검색
results = manager.find_collection_by_signal_name("Signal_A")
for result in results:
    result.display_signals()
