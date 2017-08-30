#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

std::pair<bool, double> parseNumericScalePostfix(const QString& str)
{
    static QMap<QString, double> scales{
        {"", 1.0e+0},
        {"G", 1.0e+9}, {"M", 1.0e+6}, {"k", 1.0e+3},
        {"m", 1.0e-3}, {"u", 1.0e-6}, {"n", 1.0e-9},
        {"p", 1.0e-12}, {"f", 1.0e-15}
    };

    QRegularExpression re("(\\+?[0-9]*\\.?[0-9]*(?:[eE][-+]?[0-9]+)?)([GMkmunpf])?");
    QRegularExpressionMatch match = re.match(str);
    if (match.hasMatch())
    {
        QLocale c{QLocale::C};
        double value = c.toDouble(match.captured(1));
        if (match.lastCapturedIndex() >= 2)
            value = value * scales[match.captured(2)];
        return std::make_pair(true, value);
    }

    return std::make_pair(false, 0.);
}

std::pair<double, QString> getNumericScalePostfix(double value)
{
    if (value > 1.0e+9) {
        return std::make_pair(value / 1.0e+9, "G");
    } else if (value > 1.0e+6) {
        return std::make_pair(value / 1.0e+6, "M");
    } else if (value > 1.0e+3) {
        return std::make_pair(value / 1.0e+3, "k");
    } else if (value > 1.0e+0) {
        return std::make_pair(value, "");
    } else if (value > 1.0e-3) {
        return std::make_pair(value / 1.0e-3, "m");
    } else if (value > 1.0e-6) {
        return std::make_pair(value / 1.0e-6, "u");
    } else if (value > 1.0e-9) {
        return std::make_pair(value / 1.0e-9, "n");
    } else if (value > 1.0e-12) {
        return std::make_pair(value / 1.0e-12, "p");
    } else {
        return std::make_pair(value / 1.0e-15, "f");
    }
}

QString toPostfixedString(double value) {
    auto scaled = getNumericScalePostfix(value);
    return QString("%1%2").arg(scaled.first).arg(scaled.second);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnClear_clicked()
{
    ui->txtInductance->clear();
    ui->txtCapacitance->clear();
    ui->txtFrequency->clear();
}

void MainWindow::on_pbInductance_clicked()
{
    auto cap = parseNumericScalePostfix(ui->txtCapacitance->text());
    auto freq = parseNumericScalePostfix(ui->txtFrequency->text());

    if (cap.first && freq.first)
    {
        auto ind = 1./(std::pow(2.*M_PI*freq.second, 2.)*cap.second);
        ui->txtInductance->setText(toPostfixedString(ind));
    }
}

void MainWindow::on_pbCapacitance_clicked()
{
    auto ind = parseNumericScalePostfix(ui->txtInductance->text());
    auto freq = parseNumericScalePostfix(ui->txtFrequency->text());

    if (ind.first && freq.first)
    {
        auto cap = 1./(std::pow(2.*M_PI*freq.second, 2.)*ind.second);
        ui->txtCapacitance->setText(toPostfixedString(cap));
    }
}

void MainWindow::on_pbFrequency_clicked()
{
    auto ind = parseNumericScalePostfix(ui->txtInductance->text());
    auto cap = parseNumericScalePostfix(ui->txtCapacitance->text());

    if (ind.first && cap.first)
    {
        auto freq= 1./(2.*M_PI*std::sqrt(ind.second*cap.second));
        ui->txtFrequency->setText(toPostfixedString(freq));
    }
}
