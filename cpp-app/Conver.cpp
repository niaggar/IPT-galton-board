#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

#define N 1
#define NS 100
#define NP 100
#define No 1
#define Nis 100
#define NR 1000
#define NPT 500000
#define ncut 1
#define Ro 2
#define alphaR 0.4
#define alphaO 0.4

double Temp, Gamma, Gsed, sigma, eps;
double x[NP], y[NP], Vx[NP], Vy[NP], xo[NS], yo[NP], Vxo[NP], Vyo[NP];
double DYDTvx[NP], DYDTvy[NP], fx[NP], fy[NP], raux, de, dy, vr, vt;
double xs[3*NS+1], ys[3*NS+1], per, Lx, Ly, xini, xc[No], yc[No];
double vxa, vya, Hi[Nis], Hit[Nis];
int64_t npts, mynpts;
int ierr, myid, nprocs, Ji, l;

int main() {

    std::ofstream file1("posP.dat");
    std::ofstream file2("His.dat");

    double H = 0.005;
    double T = 0;
    Gsed = 1.0;
    Lx = NS;
    Ly = NS;

    for (int i = 0; i < Nis; i++) {
        Hi[i] = 0;
        Hit[i] = 0;
    }

    npts = NR;

    if (myid == 0) {
        mynpts = npts - (nprocs-1)*(npts/nprocs);
    } else {
        mynpts = npts/nprocs;
    }

    for (int i = 0; i < 3*NS+1; i++) {
        xs[i] = 0.0;
        ys[i] = 0.0;
    }

    // Centro de los obstaculos
    for (int i = 0; i < No; i++) {
        xc[i] = 0.5 * Lx;
        yc[i] = 0.5 * Ly;
    }

    // Base de la caja
    for (int i = 0; i < NS; i++) {
        xs[i] = (i-1) + 0.5;
        ys[i] = 0.0;
    }

int Npa;
int NPa;

// Pared izquierda
for (int i = 2 * NS + 1; i <= 3 * NS; i++) {
    xs[i] = 0.5;
    ys[i] = ((i - 2 * NS) - 1) + 0.5;
}

for (int ji = 1; ji <= mynpts; ji++) {
    int idum = -ji + myid * mynpts;
    NPa = 0;

// Itera en el tiempo
for (int J = 1; J <= NPT; J++) {
    // Deposita las partículas cada cierto tiempo
    if (J / float(1000) == int(J / float(1000))) {
        Npa = Npa + 1;
        if (Npa > NP) {
            goto 10;
        }
        x[Npa] = 0.5 * Lx + sigma * 0.5;
        y[Npa] = Ly - 1;
        Vx[Npa] = 2.5 * (srand(idum) - 0.5);
        Vy[Npa] = 2.5 * (srand(idum) - 0.5);
    }

        // Integra las EC de Mov
        for (int i = 1; i <= Npa; i++) {
            vx[i] = vx[i];
            if (y[i] > 0) {
                vy[i] = vy[i] - Gsed * H;
            }
            x[i] = x[i] + vx[i] * H;
            y[i] = y[i] + vy[i] * H;

            // Colision con la base
            if (y[i] < 0.0) {
                vx[i] = vx[i] * alphaR * 0.1;
                vy[i] = -vy[i] * alphaR;
                y[i] = 0.0;
            }

            // Colision con la pared izquierda
            if (x[i] < 0.5) {
                vx[i] = abs(vx[i]) * alphaR;
                vy[i] = vy[i]; // *alphaR
                x[i] = 0.5;
            }

            // Colision con la pared derecha
            if (x[i] > NS - 0.5) {
                vx[i] = -abs(vx[i]) * alphaR;
                vy[i] = vy[i]; // *alphaR
                x[i] = NS - 0.5;
            }
        }
    }
}


    // Colision con los obstaculos
    for (int l = 1; l <= No; l++) {
        double raux = sqrt(pow(xc[l] - x[i], 2) + pow(yc[l] - y[i], 2));
        double dx = -xc[l] + x[i];
        double dy = -yc[l] + y[i];
        double vxa = vx[i];
        double vya = vy[i];
        if (raux < Ro) {
            double vr = -alphaO * (vxa * dx + vya * dy) / raux;
            double vt = (-vxa * dy + vya * dx) / raux;
            vx[i] = vr * dx / raux - vt * dy / raux;
            vy[i] = vr * dy / raux + vt * dx / raux;
            x[i] = Ro * dx / raux + xc[l];
            y[i] = Ro * dy / raux + yc[l];
        }
    }

    // Escribe el archivo de ovito
    if (J / 10 == J / 10.0 && NR == 1) {
        std::cout << 3 * Ns + NPa + No << std::endl;
        std::cout << "aver" << std::endl;
        for (int i = 1; i <= 3 * NS; i++) {
            std::cout << i << " 2 " << xs[i] << " " << ys[i] << " 1" << std::endl;
        }
        for (int i = 1; i <= NPa; i++) {
            std::cout << 3 * Ns + i << " 1 " << x[i] << " " << y[i] << " 1" << std::endl;
        }
        for (int l = 1; l <= No; l++) {
            std::cout << 3 * Ns + Npa + i << " 3 " << xc[l] << " " << yc[l] << " 1" << std::endl;
        }
    }

    // Ciclo tiempo
    for (int i = 1; i <= Npa; i++) {
        Hi[int(x[i]) + 1] += 1;
    }

    return 0;

}