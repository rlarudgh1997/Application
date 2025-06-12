import os
import shutil
import argparse

def overwrite_a_with_b_xlsx_files_consider_negative(a_root, b_root):
    for dirpath, _, filenames in os.walk(b_root):
        for filename in filenames:
            if not filename.endswith('.xlsx'):
                continue

            b_file_path = os.path.join(dirpath, filename)
            rel_path = os.path.relpath(b_file_path, b_root)
            a_file_path = os.path.join(a_root, rel_path)

            # 1. A에 동일한 파일명이 있으면 덮어쓰기
            if os.path.exists(a_file_path):
                print(f"[덮어쓰기] {a_file_path} ← {b_file_path}")
                shutil.copy2(b_file_path, a_file_path)
                continue

            # 2. A에 _Negative 붙은 파일이 있으면 그것을 덮어쓰기
            name, ext = os.path.splitext(rel_path)
            negative_rel_path = f"{name}_Negative{ext}"
            a_negative_file_path = os.path.join(a_root, negative_rel_path)

            if os.path.exists(a_negative_file_path):
                print(f"[덮어쓰기 -Negative] {a_negative_file_path} ← {b_file_path}")
                shutil.copy2(b_file_path, a_negative_file_path)
            else:
                print(f"[스킵] 해당 파일 없음: {a_file_path} 또는 {a_negative_file_path}")

def main():
    parser = argparse.ArgumentParser(description="B 경로의 .xlsx 파일로 A 경로 파일을 덮어쓰기 (Negative 대응)")
    parser.add_argument("a_path", help="덮어쓸 대상 경로 (A)")
    parser.add_argument("b_path", help="기준이 되는 원본 경로 (B)")

    args = parser.parse_args()
    a_path = args.a_path
    b_path = args.b_path

    if not os.path.isdir(a_path) or not os.path.isdir(b_path):
        print("❌ 경로 오류: A 또는 B 경로가 존재하지 않습니다.")
        return

    print(f"🔁 A 경로: '{a_path}'\n🔁 B 경로: '{b_path}'")
    overwrite_a_with_b_xlsx_files_consider_negative(a_path, b_path)

if __name__ == "__main__":
    main()