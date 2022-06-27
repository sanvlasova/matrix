#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

// ����� ����������� ������
class Matrix {
    struct Node {
        double value; // �������� ��������
        int i; // ������ ��������
        int j; // ������� ��������
    };

    int n; // ���������� �����
    int m; // ���������� ��������
    vector<Node> nodes; // �������� �������

    int GetNode(int i, int j) const; // ��������� ������� ��������
    double MaxInRow(int i) const; // �������� � ������
    double MinInColumn(int j) const; // ������� � �������

    vector<int> RowArgmin() const; // ������ �������� ��������� � �������
    vector<int> RowArgmax() const; // ������ �������� ���������� � �������
    vector<int> ColumnArgmin() const; // ������ �������� ��������� � ��������
    vector<int> ColumnArgmax() const; // ������ �������� ���������� � ��������
public:
    Matrix(int n, int m); // ����������� �� ��������

    double Get(int i, int j) const; // ��������� �������� ��������
    void Set(int i, int j, double value); // ���������� ��������

    double Sum() const; // ����� ���������
    double Product() const; // ������������ ���������
    double Max() const; // ��������
    double Min() const; // �������

    vector<double> SequenceByRows() const; // ������������������ ��� ������ �� �������
    vector<double> SequenceByColumns() const; // ������������������ ��� ������ �� ��������

    double MinOfMaxInRows() const; // ������� �� ���������� �� �������
    double MaxOfMinInColumns() const; // �������� �� ��������� �� ��������

    void ShowLocalMaximums() const; // ����������� ��������� ����������
    void ShowLocalMinimums() const; // ����������� ��������� ���������
    void ShowSeddlePoints() const; // ����������� �������� �����

    Matrix GetBlock(int i1, int i2, int j1, int j2) const; // ��������� �����
    void Print() const; // ����� ������� 
};

// ����������� �� ��������
Matrix::Matrix(int n, int m) {
    this->n = n; // ���������� ������
    this->m = m; // ���������� �������
}

// ��������� ������� ��������
int Matrix::GetNode(int i, int j) const {
    for (int index = 0; index < nodes.size(); index++)
        if (nodes[index].i == i && nodes[index].j == j) // ���� ����� �������
            return index;

    return -1; // �� �����
}

// �������� � ������
double Matrix::MaxInRow(int i) const {
    int index = 0;

    // ���� ��������� ������� � �������� ������
    while (index < nodes.size() && nodes[index].i != i)
        index++;

    if (index == nodes.size()) // ���� ����� ��������� ���
        return 0; // �� ����

    double max = nodes[index].value;

    for (; index < nodes.size(); index++)
        if (nodes[index].i == i && nodes[index].value > max) // ���� ������� � ��� �� ������ � ������ ���������
            max = nodes[index].value; // �� ���������

    return max;
}

// ������� � �������
double Matrix::MinInColumn(int j) const {
    int index = 0;

    // ���� ��������� ������� � �������� �������
    while (index < nodes.size() && nodes[index].j != j)
        index++;

    if (index == nodes.size()) // ���� ����� ��������� ���
        return 0; // �� ����

    double min = nodes[index].value;

    for (; index < nodes.size(); index++)
        if (nodes[index].j == j && nodes[index].value < min) // ���� ������� � ��� �� ������� � ������ ��������
            min = nodes[index].value; // �� ���������

    return min;
}

// ������ �������� ��������� � �������
vector<int> Matrix::RowArgmin() const {
    vector<int> mins(n, 0);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (Get(i, j) < Get(i, mins[i]))
                mins[i] = j;

    return mins;
}

// ������ �������� ���������� � �������
vector<int> Matrix::RowArgmax() const {
    vector<int> maxs(n, 0);

    for (int i = 0; i < n; i++)
        for (int j = 1; j < m; j++)
            if (Get(i, j) > Get(i, maxs[i]))
                maxs[i] = j;

    return maxs;
}

// ������ �������� ��������� � ��������
vector<int> Matrix::ColumnArgmin() const {
    vector<int> mins(m, 0);

    for (int j = 0; j < m; j++)
        for (int i = 1; i < n; i++)
            if (Get(i, j) < Get(mins[j], j))
                mins[j] = i;

    return mins;
}

// ������ �������� ���������� � ��������
vector<int> Matrix::ColumnArgmax() const {
    vector<int> maxs(n, 0);

    for (int j = 0; j < m; j++)
        for (int i = 1; i < n; i++)
            if (Get(i, j) > Get(maxs[j], j))
                maxs[j] = i;

    return maxs;
}

// ��������� �������� ��������
double Matrix::Get(int i, int j) const {
    int index = GetNode(i, j); // ���� �������    

    return index > -1 ? nodes[index].value : 0; // ���������� ��������
}

// ���������� ��������
void Matrix::Set(int i, int j, double value) {
    int index = GetNode(i, j); // ���� �������

    if (index == -1 && value == 0) // ���� �������� ��� � �������� �������
        return; // �� �������

    if (index > -1) { // ���� �����
        if (value != 0) // ���� �������� �� �������
            nodes[index].value = value; // �� ��������� ���
        else
            nodes.erase(nodes.begin() + index); // ������� ���� �������
    }
    else { // �������� ���, ����� ���������
        Node node;
        node.i = i;
        node.j = j;
        node.value = value;
        nodes.push_back(node); // ��������� �������
    }
}

// ����� ���������
double Matrix::Sum() const {
    double sum = 0;

    for (int i = 0; i < nodes.size(); i++)
        sum += nodes[i].value; // ���������� ������ ��������� �������

    return sum;
}

// ������������ ���������
double Matrix::Product() const {
    if (nodes.size() < n * m) // ���� ���� ����
        return 0; // ���������� ����

    double prod = 1;

    for (int i = 0; i < nodes.size(); i++)
        prod *= nodes[i].value; // �������� �� ��� ��������� ��������

    return prod;
}

// ��������
double Matrix::Max() const {
    if (nodes.size() == 0) // ���� ��� ��������� ���������
        return 0; // �� ����

    double max = nodes[0].value; // ������� ���������� ������ �������

    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i].value > max) // ���� ������� ������ ���������
            max = nodes[i].value; // ��������� ��������

    return max > 0 ? max : 0; // ���������� �������� �� ���� � ������������� ��������
}

// �������
double Matrix::Min() const {
    if (nodes.size() == 0) // ���� ��� ��������� ���������
        return 0; // �� ����

    double min = nodes[0].value; // ������� ��������� ������ �������

    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i].value < min) // ���� ������� ������ ��������
            min = nodes[i].value; // ��������� �������

    return min < 0 ? min : 0; // ���������� ������� �� ���� � ������������ ��������
}

// ������������������ ��� ������ �� �������
vector<double> Matrix::SequenceByRows() const {
    vector<double> sequence; // ������ ������ ��� ������������������

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            sequence.push_back(Get(i, j)); // ��������� � ������������������ ��������� �������

    return sequence; // ���������� ������������������
}

// ������������������ ��� ������ �� ��������
vector<double> Matrix::SequenceByColumns() const {
    vector<double> sequence; // ������ ������ ��� ������������������

    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            sequence.push_back(Get(i, j)); // ��������� � ������������������ ��������� �������

    return sequence; // ���������� ������������������
}

// ������� �� ���������� �� �������
double Matrix::MinOfMaxInRows() const {
    double min = MaxInRow(0); // ���� �������� � ������� ������

    for (int i = 1; i < n; i++) {
        double max = MaxInRow(i); // �������� �������� � ������

        if (max < min) // ���� �� ������ ��������
            min = max; // ��������� �������
    }

    return min; // ���������� �������
}

// �������� �� ��������� �� ��������
double Matrix::MaxOfMinInColumns() const {
    double max = MinInColumn(0); // ���� ������� � ������� �������

    for (int j = 1; j < m; j++) {
        double min = MinInColumn(j); // �������� ������� � �������

        if (min > max) // ���� �� ������ ���������
            max = min; // ��������� ��������
    }

    return max; // ���������� ��������
}

// ����������� ��������� ����������
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
                    continue; // ���������� ������������ ������

                if (Get(ii, jj) > value) // ���� ���� �������� ������
                    isLocal = false; // �� ��� �� ��������� ��������
            }

            if (isLocal) {
                cout << "Find local maximum at " << i << " " << j << ": " << value << endl; // ������� ��������
            }
        }
    }
}

// ����������� ��������� ���������
void Matrix::ShowLocalMinimums() const {
    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double value = Get(i, j);
            bool isLocal = true;

            // ��� �� ���� �������
            for (int k = 0; k < 8 && isLocal; k++) {
                int ii = i + dy[k];
                int jj = j + dx[k];

                if (ii < 0 || ii >= n || jj < 0 || jj >= m)
                    continue; // ���������� ������������ ������

                if (Get(ii, jj) < value) // ���� ���� �������� ������
                    isLocal = false; // �� ��� �� ��������� �������
            }

            if (isLocal) {
                cout << "Find local minimum at " << i << " " << j << ": " << value << endl; // ������� �������
            }
        }
    }
}

// ����������� �������� �����
void Matrix::ShowSeddlePoints() const {
    // �������� ������� ��������� � ���������� �� �������
    vector<int> rowMins = RowArgmin();
    vector<int> rowMaxs = RowArgmax();

    // �������� ������� ��������� � ���������� �� ��������
    vector<int> columnMins = ColumnArgmin();
    vector<int> columnMaxs = ColumnArgmax();     

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (rowMins[i] == j && columnMaxs[j] == i || rowMaxs[i] == j && columnMins[j] == i)
                cout << "Seddle point at " << i << " " << j << ": " << Get(i, j) << endl;
}

// ����� �������
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

// ��������� �����
Matrix Matrix::GetBlock(int i1, int i2, int j1, int j2) const {
    Matrix block(i2 - i1 + 1, j2 - j1 + 1);

    for (int i = 0; i < nodes.size(); i++)
        if (nodes[i].i >= i1 && nodes[i].i <= i2 && nodes[i].j >= j1 && nodes[i].j <= j2)
            block.Set(nodes[i].i - i1, nodes[i].j - j1, nodes[i].value);

    return block;
}

// ����� �������
void PrintVector(const vector<double> &v) {
    for (int i = 0; i < v.size(); i++)
        cout << v[i]<< " ";
    cout << endl;
}

int main() {
    int n, m;
    cout << "Enter n and m: ";
    cin >> n >> m; // ��������� ������� �������

    Matrix matrix(n, m); // ������ �������

    cout << "Created matrix:" << endl;
    matrix.Print(); // ������� ��������� �������

    int count;
    cout << "Enter number of values: ";
    cin >> count; // ������ ��������� ����� ��� �����

    for (int index = 0; index < count; index++) {
        int i, j;
        double value;
        cout << "Enter i, j and value: ";
        cin >> i >> j >> value; // ��������� ������� � ��������

        matrix.Set(i, j, value); // ������������� ��������
    }

    cout << "Readed matrix:" << endl;
    matrix.Print(); // ������� �������� �������

    int i1, i2, j1, j2;
    cout << "Enter block (i1 i1 j1 j2): ";
    cin >> i1 >> i2 >> j1 >> j2; // ��������� ���������� �����

    Matrix block = matrix.GetBlock(i1, i2, j1, j2); // �������� ����
    cout << "Block: " << endl;
    block.Print(); // ������� ����


    cout << "Sum: " << block.Sum() << endl; // ������� �����
    cout << "Product: " << block.Product() << endl; // ������� ������������
    cout << "Min: " << block.Min() << endl; // ������� �������
    cout << "Max: " << block.Max() << endl; // ������� ��������

    cout << "Sequence by rows: ";
    PrintVector(block.SequenceByRows()); // ������� ������������������ �� �������
    cout << "Sequence by columns: ";
    PrintVector(block.SequenceByColumns()); // ������� ������������������ �� ��������

    cout << endl << "Local maximums:" << endl;
    block.ShowLocalMaximums(); // ������� ��������� ���������
    cout << endl << "Local minimums:" << endl;
    block.ShowLocalMinimums(); // ������� ��������� ��������

    cout << endl << "Min of max in rows: " << block.MinOfMaxInRows() << endl; // ������� ������� �� ���������� � �������
    cout << "Max of min in columns: " << block.MaxOfMinInColumns() << endl << endl; // ������� �������� �� ��������� � ��������

    cout << "Seddle points:"<< endl;
    block.ShowSeddlePoints(); // ������� ��������� �����
}