#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <mmsystem.h>
#include <QTimer>
#include "QGraphicsScene"
#include "MidiFile.h"
#include "Options.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_uploadButton_clicked();

    void on_playButton_clicked();

    void on_stopButton_clicked();

    void updateSlider();

    void fillNoteTable(const smf::MidiFile &midi, int track);

    void drawPianoRoll(smf::MidiFile &midi, int track);

    void on_comboBox_tracks_currentIndexChanged(int index);

    void on_deleteTrackbtn_clicked();

    void on_saveButton_clicked();

    void stopbtn_ShowEnab(bool state);

    void playbtn_ShowEnab(bool state);

    void deleteTbtn_ShowEnab(bool state);

    void reloadMidi(int trackToSelect = -1);

    void saveMidi(const QString &outPath);

    void saveButton_ShowEnab(bool state);

    void on_addTrackbtn_clicked();

    void on_mergeOpenbtn_clicked();

    void on_mergeTrackbtn_clicked();

    void updateTrackBoxes();

private:
    Ui::MainWindow *ui;
    QByteArray midiData;
    QString path;
    QTimer *timer;
    QGraphicsScene *scene;

    smf::MidiFile currentMidi;

};
#endif // MAINWINDOW_H
