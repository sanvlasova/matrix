#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// класс разреженных матриц
class Matrix {
    struct Node {
        double value; // значение элемента
        int i; // строка элемента
        int j; // столбец элемента
    };

    int n; // количество строк
    int m; // количество столбцов
    vector<Node> nodes; // элементы матрицы

    int GetNode(int i, int j) const; // получение индекса элемента
    double MaxInRow(int i) const; // максимум в строке
    double MinInColumn(int j) const; // минимум в столбце

    vector<int> RowArgmin() const; // вектор индексом минимумов в строках
    vector<int> RowArgmax() const; // вектор индексом максимумов в строках
    vector<int> ColumnArgmin() const; // вектор индексом минимумов в столбцах
    vector<int> ColumnArgmax() const; // вектор индексом максимумов в столбцах
public:
    Matrix(int n, int m); // конструктор из размеров

    double Get(int i, int j) const; // получение значения элемента
    void Set(int i, int j, double value); // установить значение

    double Sum() const; // сумма элементов
    double Product() const; // произведение элементов
    double Max() const; // максимум
    double Min() const; // минимум

    vector<double> SequenceByRows() const; // последовательность при обходе по строкам
    vector<double> SequenceByColumns() const; // последовательность при обходе по столбцам

    double MinOfMaxInRows() const; // минимум из максимумов по строкам
    double MaxOfMinInColumns() const; // максимум из минимумом по столбцам

    void ShowLocalMaximums() const; // отображение локальных максимумом
    void ShowLocalMinimums() const; // отображение локальных минимумов
    void ShowSeddlePoints() const; // отображение седловых точек

    Matrix GetBlock(int i1, int i2, int j1, int j2) const; // получение блока
    void Print() const; // вывод матрицы 
};

// конструктор из размеров
Matrix::Matrix(int n, int m) {
    this->n = n; // запоминаем строки
    this->m = m; // запоминаем столбцы
}

// получение индекса элемента
int Matrix::GetNode(int i, int j) const {
    for (int index = 0; index < nodes.size(); index++)
        if (nodes[index].i == i && nodes[index].j == j) // если нашли элемент
            return index;

    return -1; // не нашли
}

// максимум в строке
double Matrix::MaxInRow(int i) const {
    int index = 0;

    // ищем ненулевой элемент в заданной строке
    while (index < nodes.size() && nodes[index].i != i)
        index++;

    if (index == nodes.size()) // если таких элементов нет
        return 0; // то ноль

    double max = nodes[index].value;

    for (; index < nodes.size(); index++)
        if (nodes[index].i == i && nodes[index].value > max) // если элемент в той же строке и больше максимума
            max = nodes[index].value; // то обновляем

    return max;
}

// минимум в столбце
double Matrix::MinInColumn(int j) const {
    int index = 0;

    // ищем ненулевой элемент в заданном столбце
    while (index < nodes.size() && nodes[index].j != j)
        index++;

    if (index == nodes.size()) // если таких элементов нет
        return 0; // то ноль

    double min = nodes[index].value;

    for (; index < nodes.size(); index++)
        if (nodes[index].j == j && nodes[index].value < min) // если элемент в том же столбце и меньше минимума
            min = nodes[index].value; // то обновляем

    return min;
}

// вектор индексом минимумов в строках
vector<int> Matrix::RowArgmin() const {
    vector<int> mins(n, 0);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (Get(i, j) < Get(i, mins[i]))
                mins[i] = j;

    return mins;
}

// вектор индексом максимумов в строках
vector<int> Matrix::RowArgmax() const {
    vector<int> maxs(n, 0);

    for (int i = 0; i < n; i++)
        for (int j = 1; j < m; j++)
            if (Get(i, j) > Get(i, maxs[i]))
                maxs[i] = j;

    return maxs;
}

// вектор индексом минимумов в столбцах
vector<int> Matrix::ColumnArgmin() const {
    vector<int> mins(m, 0);

    for (int j = 0; j < m; j++)
        for (int i = 1; i < n; i++)
            if (Get(i, j) < Get(mins[j], j))
                mins[j] = i;

    return mins;
}

// вектор индексом максимумов в столбцах
vector<int> Matrix::ColumnArgmax() const {
    vector<int> maxs(n, 0);

    for (int j = 0; j < m; j++)
        for (int i = 1; i < n; i++)
            if (Get(i, j) > Get(maxs[j], j))
                maxs[j] = i;

    return maxs;
}

// получение значения элемента
double Matrix::Get(int i, int j) const {
    int index = GetNode(i, j); // ищем элемент    

    return index > -1 ? nodes[index].value : 0; // возвращаем значение
}

// установить значение
void Matrix::Set(int i, int j, double value) {
    int index = GetNode(i, j); // ищем элемент

    if (index == -1 && value == 0) // если элемента нет и значение нулевое
        return; // то вызодим

    if (index > -1) { // если нашли
        if (value != 0) // если значение не нулевое
            nodes[index].value = value; // то обновляем его
        else
            nodes.erase(nodes.begin() + index); // удаляем этот элемент
    }
    else { // элемента нет, число ненулевое
        Node node;
        node.i = i;
        node.j = j;
        node.value = value;
        nodes.push_back(node); // вставляем элемент
    }
}

// сумма элементов
double Matrix::Sum() const {
    double sum = 0;

    for (int i = 0; i < nodes.size(); i++)
        sum += nodes[i].value; // прибавляем каждый ненулевой элемент

    return sum;
}

// произведение элементов
double Matrix::Product() const {
    if (nodes.size() < n * m) // если есть нули
        return 0; // возвращаем ноль

    double prod = 1;

    for (int i = 0; i < nodes.size(); i++)
        prod *= nodes[i].value; // умножаем на все ненулевые элементы

    return prod;
}

// максимум
double Matrix::Max() const {
    if (nodes.size() == 0) // если нет ненулевых элементов
        return 0; // то ноль

    double max = nodes[0].value; // считаем максимумом первый элемент

    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i].value > max) // если элемент больше максимума
            max = nodes[i].value; // обновляем максимум

    return max > 0 ? max : 0; // возвращаем максимум из нуля и максимального элемента
}

// минимум
double Matrix::Min() const {
    if (nodes.size() == 0) // если нет ненулевых элементов
        return 0; // то ноль

    double min = nodes[0].value; // считаем минимумом первый элемент

    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i].value < min) // если элемент меньше минимума
            min = nodes[i].value; // обновляем минимум

    return min < 0 ? min : 0; // возвращаем минимум из нуля и минимального элемента
}

// последовательность при обходе по строкам
vector<double> Matrix::SequenceByRows() const {
    vector<double> sequence; // создаём веткор для последовательности

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            sequence.push_back(Get(i, j)); // добавляем в последовательность очерденой элемент

    return sequence; // возвращаем последовательность
}

// последовательность при обходе по столбцам
vector<double> Matrix::SequenceByColumns() const {
    vector<double> sequence; // создаём веткор для последовательности

    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            sequence.push_back(Get(i, j)); // добавляем в последовательность очерденой элемент

    return sequence; // возвращаем последовательность
}

// минимум из максимумов по строкам
double Matrix::MinOfMaxInRows() const {
    double min = MaxInRow(0); // ищем максимум в нулевой строке

    for (int i = 1; i < n; i++) {
        double max = MaxInRow(i); // получаем максимум в строке

        if (max < min) // если он меньше минимума
            min = max; // обновляем минимум
    }

    return min; // возвращаем минимум
}

// максимум из минимумом по столбцам
double Matrix::MaxOfMinInColumns() const {
    double max = MinInColumn(0); // ищем минимум в нулевом столбце

    for (int j = 1; j < m; j++) {
        double min = MinInColumn(j); // получаем минимум в столбце

        if (min > max) // если он больше максимума
            max = min; // обновляем максимум
    }

    return max; // возвращаем максимум
}

// отображение локальных максимумом
void Matrix::ShowLocalMaximums() const {
    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double value = Get(i, j);
            bool isLocal = true;

            for (int k = 0; k < 8 && isLocal; k++) {
                int ii = i + dy[k];
                int jj = j + dx[k];

                if (ii < 0 || ii >= n || jj < 0 || jj >= m)
                    continue; // игнорируем некорректные клетки

                if (Get(ii, jj) > value) // если есть значение больше
                    isLocal = false; // то это не локальный максимум
            }

            if (isLocal) {
                cout << "Find local maximum at " << i << " " << j << ": " << value << endl; // выводим максимум
            }
        }
    }
}

// отображение локальных минимумов
void Matrix::ShowLocalMinimums() const {
    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double value = Get(i, j);
            bool isLocal = true;

            // идём по всем соседям
            for (int k = 0; k < 8 && isLocal; k++) {
                int ii = i + dy[k];
                int jj = j + dx[k];

                if (ii < 0 || ii >= n || jj < 0 || jj >= m)
                    continue; // игнорируем некорректные клетки

                if (Get(ii, jj) < value) // если есть значение меньше
                    isLocal = false; // то это не локальный минимум
            }

            if (isLocal) {
                cout << "Find local minimum at " << i << " " << j << ": " << value << endl; // выводим минимум
            }
        }
    }
}

// отображение седловых точек
void Matrix::ShowSeddlePoints() const {
    // получаем индексы минимумов и максимумов по строкам
    vector<int> rowMins = RowArgmin();
    vector<int> rowMaxs = RowArgmax();

    // получаем индексы минимумов и максимумов по столбцам
    vector<int> columnMins = ColumnArgmin();
    vector<int> columnMaxs = ColumnArgmax();     

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (rowMins[i] == j && columnMaxs[j] == i || rowMaxs[i] == j && columnMins[j] == i)
                cout << "Seddle point at " << i << " " << j << ": " << Get(i, j) << endl;
}

// вывод матрицы
void Matrix::Print() const {
    cout << "Matrix as elements:" << endl;
    for (int i = 0; i < nodes.size(); i++)
        cout << "    " << nodes[i].value << " at [" << nodes[i].i << ", " << nodes[i].j << "]" << endl;
    
    cout << endl;

    cout << "Matrix as matrix:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "    ";

        for (int j = 0; j < m; j++)
            cout << setw(5) << Get(i, j) << " ";

        cout << endl;
    }
}

// получение блока
Matrix Matrix::GetBlock(int i1, int i2, int j1, int j2) const {
    Matrix block(i2 - i1 + 1, j2 - j1 + 1);

    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i].i >= i1 && nodes[i].i <= i2 && nodes[i].j >= j1 && nodes[i].j <= j2)
            block.Set(nodes[i].i - i1, nodes[i].j - j1, nodes[i].value);

    return block;
}

// вывод вектора
void PrintVector(const vector<double> &v) {
    for (int i = 0; i < v.size(); i++)
        cout << v[i]<< " ";
    cout << endl;
}

int main() {
    int n, m;
    cout << "Enter n and m: ";
    cin >> n >> m; // считываем размеры матрицы

    Matrix matrix(n, m); // создаём матрицу

    cout << "Created matrix:" << endl;
    matrix.Print(); // выводим созданную матрицу

    int count;
    cout << "Enter number of values: ";
    cin >> count; // вводим количесво чисел для ввода

    for (int index = 0; index < count; index++) {
        int i, j;
        double value;
        cout << "Enter i, j and value: ";
        cin >> i >> j >> value; // считываем позицию и значение

        matrix.Set(i, j, value); // устанавливает значение
    }

    cout << "Readed matrix:" << endl;
    matrix.Print(); // выводим введённую матрицу

    int i1, i2, j1, j2;
    cout << "Enter block (i1 i1 j1 j2): ";
    cin >> i1 >> i2 >> j1 >> j2; // считываем координаты блока

    Matrix block = matrix.GetBlock(i1, i2, j1, j2); // получаем блок
    cout << "Block: " << endl;
    block.Print(); // выводим блок


    cout << "Sum: " << block.Sum() << endl; // выводим сумму
    cout << "Product: " << block.Product() << endl; // выводим произведение
    cout << "Min: " << block.Min() << endl; // выводим минимум
    cout << "Max: " << block.Max() << endl; // выводим максимум

    cout << "Sequence by rows: ";
    PrintVector(block.SequenceByRows()); // выводим последовательность по строкам
    cout << "Sequence by columns: ";
    PrintVector(block.SequenceByColumns()); // выводим последовательность по столбцам

    cout << endl << "Local maximums:" << endl;
    block.ShowLocalMaximums(); // выводим локальные максимумы
    cout << endl << "Local minimums:" << endl;
    block.ShowLocalMinimums(); // выводим локальные минимумы

    cout << endl << "Min of max in rows: " << block.MinOfMaxInRows() << endl; // выводим минимум из максимумов в строках
    cout << "Max of min in columns: " << block.MaxOfMinInColumns() << endl << endl; // выводим максимум из минимумов в столбцах

    cout << "Seddle points:"<< endl;
    block.ShowSeddlePoints(); // выводим седлловые точки
}