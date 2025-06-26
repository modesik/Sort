#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace std::chrono;

// Структура для хранения записей из файла
struct Record {
  int sortingfield; // Поле, по которому будем сортировать
  Record(int field) : sortingfield(field) {}
};

// Функция для проверки формата файла
bool checkFileFormat(const string &filePath, const string &expectedFormat) {
  size_t pos = filePath.find_last_of(".");
  if (pos == string::npos) {
    return false; // Файл без расширения
  }
  string fileFormat = filePath.substr(pos + 1);
  return fileFormat == expectedFormat;
}

// Функция для чтения данных из файла
bool readRecordsFromFile(const string &filePath, size_t numBytes,
                         vector<Record> &records) {
  ifstream inputFile(filePath, ios::binary);
  if (!inputFile.is_open()) {
    cerr << "Невозможно открыть входной файл" << endl;
    return false;
  }

  // Чтение указанного количества байт из файла
  stringstream buffer;
  buffer << inputFile.rdbuf();
  string fileData = buffer.str().substr(0, numBytes);

  stringstream ss(fileData);
  int fieldValue;
  while (ss >> fieldValue) {
    records.emplace_back(fieldValue);
  }

  inputFile.close();
  return true;
}

// Сортировка пузырьком
void bubbleSort(vector<Record> &arr, bool ascending) {
  int n = arr.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (ascending) {
        if (arr[j].sortingfield > arr[j + 1].sortingfield) {
          swap(arr[j], arr[j + 1]);
        }
      } else {
        if (arr[j].sortingfield < arr[j + 1].sortingfield) {
          swap(arr[j], arr[j + 1]);
        }
      }
    }
  }
}

// Быстрая сортировка
void quickSort(vector<Record> &arr, int low, int high, bool ascending) {
  if (low < high) {
    int pivot = arr[high].sortingfield;
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
      if (ascending) {
        if (arr[j].sortingfield < pivot) {
          i++;
          swap(arr[i], arr[j]);
        }
      } else {
        if (arr[j].sortingfield > pivot) {
          i++;
          swap(arr[i], arr[j]);
        }
      }
    }
    swap(arr[i + 1], arr[high]);
    int pi = i + 1;
    quickSort(arr, low, pi - 1, ascending);
    quickSort(arr, pi + 1, high, ascending);
  }
}

// Сортировка Шелла
void shellSort(vector<Record> &arr, bool ascending) {
  int n = arr.size();
  for (int gap = n / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < n; i++) {
      Record temp = arr[i];
      int j;
      for (j = i;
           j >= gap &&
           ((ascending && arr[j - gap].sortingfield > temp.sortingfield) ||
            (!ascending && arr[j - gap].sortingfield < temp.sortingfield));
           j -= gap) {
        arr[j] = arr[j - gap];
      }
      arr[j] = temp;
    }
  }
}

// Медленная сортировка
void slowSort(vector<Record> &arr, int low, int high, bool ascending) {
  if (low >= high) {
    return;
  }
  int mid = (low + high) / 2;
  slowSort(arr, low, mid, ascending);
  slowSort(arr, mid + 1, high, ascending);
  if (ascending) {
    if (arr[mid].sortingfield > arr[high].sortingfield) {
      swap(arr[mid], arr[high]);
    }
  } else {
    if (arr[mid].sortingfield < arr[high].sortingfield) {
      swap(arr[mid], arr[high]);
    }
  }
  slowSort(arr, low, high - 1, ascending);
}

int main() {
  setlocale(LC_ALL, "Russian");
  int typesort;
  cout << "Выберите метод сортировки (1 - Сортировка пузырьком, 2 - Быстрая "
          "сортировка, 3 - Сортировка Шелла, 4 - Медленная сортировка): ";
  cin >> typesort;

  int sortDirection;
  cout << "Выберите направление сортировки (1 - по возрастанию, 2 - по "
          "убыванию): ";
  cin >> sortDirection;

  bool ascending = (sortDirection == 1);

  string inputFilePath = "sort.txt";
  string outputFilePath = "sortsort.txt";

  // Запрос количества байт для чтения из файла
  size_t numBytes;
  cout << "Введите количество байт для чтения из файла: ";
  cin >> numBytes;

  vector<Record> records;

  // Проверка формата файла
  if (!checkFileFormat(inputFilePath, "txt")) {
    cerr << "Неверный формат входного файла. Ожидается формат .txt" << endl;
    return 1;
  } else {
    cout << "Файл успешно прочитан. " << endl;
  }

  // Чтение данных из файла
  if (!readRecordsFromFile(inputFilePath, numBytes, records)) {
    return 1; // Произошла ошибка при чтении файла
  }

  cout << "Количество записей, считанных из входного файла: " << records.size()
       << endl;

  auto start = high_resolution_clock::now();

  // Выбор метода сортировки и сортировка данных
  switch (typesort) {
  case 1:
    bubbleSort(records, ascending);
    break;
  case 2:
    quickSort(records, 0, records.size() - 1, ascending);
    break;
  case 3:
    shellSort(records, ascending);
    break;
  case 4:
    slowSort(records, 0, records.size() - 1, ascending);
    break;
  default:
    cout << "Неверный тип сортировки";
    return 1;
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(stop - start);
  cout << "Время сортировки: " << duration.count() << " наносекунд" << endl;
  // Вывод отсортированных данных на экран
  cout << "Отсортированные данные: ";
  for (const auto &record : records) {
    cout << record.sortingfield << " ";
  }
  cout << endl;

  // Сохранение отсортированных данных в выходной файл
  ofstream outputfile(outputFilePath);
  if (outputfile.is_open()) {
    for (size_t i = 0; i < records.size(); ++i) {
      outputfile << records[i].sortingfield;
      if (i != records.size() - 1) {
        outputfile << " ";
      }
    }
    outputfile.close();
    cout << "Отсортированные данные записаны в " << outputFilePath << endl;
  } else {
    cerr << "Невозможно открыть выходной файл" << endl;
    return 1;
  }

  return 0;
}
