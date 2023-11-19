#include <iostream>

using namespace std;

const int N = 200;            // размер доски и число ферзей
const double Tn = 10000.0;    // начальная температура
const double Tk = 0.001;      // конечная температура
const double Alfa = 0.98;     // скорость охлаждния
const int ST = 100;           // число итераций при смене T

using TSolution = struct {    // решение
    int Plan[N];              // кодировка решения
    int Energy;               // энергия решения    
};

TSolution Current;            // текущее решение
TSolution Working;            // рабочее решение
TSolution Best;               // лучшее решение


double Random(double min, double max)  // случайное число от min до max
{
    return (double)(rand()) * (max - min) / RAND_MAX + min;
}


void Swap(TSolution* M)  // модификация решения
{
    int x, y, v;
    x = Random(0, N - 1);
    do {
        y = Random(0, N - 1);
    } while (x == y);
    v = M->Plan[x];  M->Plan[x] = M->Plan[y];  M->Plan[y] = v;
}


void New(TSolution* M)  // инициализация решения
{
    int i;
    for (i = 0; i < N; i++) M->Plan[i] = i;
    for (i = 0; i < N; i++) Swap(M);
}


void CalcEnergy(TSolution* M)  // расчет энергии
{
    const int dx[4] = { -1, 1,-1, 1 };
    const int dy[4] = { -1, 1, 1,-1 };
    int j, x, tx, ty;
    int error = 0;
    for (x = 0; x < N; x++) {
        for (j = 0; j < 4; j++) {
            tx = x + dx[j];  ty = M->Plan[x] + dy[j];
            while ((tx >= 0) && (tx < N) && (ty >= 0) && (ty < N)) {
                if (M->Plan[tx] == ty) error++;
                tx += dx[j];  ty += dy[j];
            }
        }
    }
    M->Energy = error;
}


void Copy(TSolution* MD, TSolution* MS)  // копирование решения
{
    for (int i = 0; i < N; i++) MD->Plan[i] = MS->Plan[i];
    MD->Energy = MS->Energy;
}


void Show(TSolution M)  // отображение на экране
{
    int x, y;
    cout << "Решение:" << endl;
    for (y = 0; y < N; y++) {
        for (x = 0; x < N; x++)
            if (M.Plan[x] == y) cout << "Q"; else cout << ".";
        cout << endl;
    }
    cout << endl;
}


int main() {
    srand(time(0));  //setlocale(LC_ALL, "Russian");
    double  T = Tn;           // температура
    bool    fBest = false;    // флаг лучшего решения
    bool    fNew;             // флаг нового решения
    int     Time = 0;         // этап поиска
    New(&Current);          CalcEnergy(&Current);
    Copy(&Best, &Current);  Copy(&Working, &Current);
    while ((T > Tk) && (Best.Energy > 0)) {
        for (int Step = 0; Step < ST; Step++) {
            fNew = false;   Swap(&Working);   CalcEnergy(&Working);
            if (Working.Energy <= Current.Energy) fNew = true; else {
                double Delta = Working.Energy - Current.Energy;  // разница энергий
                double P = exp(-Delta / T);                      // вероятность допуска
                if (P > Random(0, 1)) fNew = true;
            }
            if (fNew) {
                fNew = false;  Copy(&Current, &Working);
                if (Current.Energy < Best.Energy) {
                    Copy(&Best, &Current);  fBest = true;
                }
            } else Copy(&Working, &Current);
        }
        cout << "Temp=" << T << " Energy=" << Best.Energy << endl;
        T *= Alfa;
        Time++;
    }
    if ((fBest) && (N < 80)) Show(Best);
    system("pause");
    return 0;
}