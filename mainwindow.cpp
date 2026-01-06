#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QMessageBox"
#include <windows.h>
#include <mmsystem.h>
#include "QList"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSlider);

    scene = new QGraphicsScene(this);
    ui->graphicsView_piano->setScene(scene);
    ui->tableWidget_notes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->playButton->setEnabled(false);
    ui->groupBoxMerge->setEnabled(false);
    ui->groupBoxMerge->setVisible(false);

    deleteTbtn_ShowEnab(0);
    saveButton_ShowEnab(0);
    stopbtn_ShowEnab(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveButton_ShowEnab(bool state)
{
    ui->saveButton->setEnabled(state);
    ui->saveButton->setVisible(state);
}

void MainWindow::deleteTbtn_ShowEnab(bool state)
{
    ui->deleteTrackbtn->setEnabled(state);
    ui->deleteTrackbtn->setVisible(state);
}

void MainWindow::playbtn_ShowEnab(bool state)
{
    ui->playButton->setEnabled(state);
    ui->playButton->setVisible(state);
}

void MainWindow::stopbtn_ShowEnab(bool state)
{
    ui->stopButton->setEnabled(state);
    ui->stopButton->setVisible(state);
}

int findFirstNoteTrack(const smf::MidiFile &midi)
{
    for (int t = 0; t < midi.getTrackCount(); t++)
    {
        for (int i = 0; i < midi[t].size(); i++)
        {
            if (midi[t][i].isNoteOn())
            {
                return t;
            }
        }
    }
    return -1;
}



void MainWindow::reloadMidi(int trackToSelect)
{
    currentMidi.doTimeAnalysis();
    currentMidi.linkNotePairs();

    if (trackToSelect == -1) {
        trackToSelect = ui->comboBox_tracks->currentData().toInt();
    }

    ui->comboBox_tracks->blockSignals(true);
    ui->comboBox_tracks->clear();

    for (int i = 0; i < currentMidi.getTrackCount(); i++)
    {
        int noteCount = 0;


        if (currentMidi[i].size() > 0)
        {
            for (int j = 0; j < currentMidi[i].size(); j++)
            {
                if (currentMidi[i][j].isNoteOn())
                {
                    noteCount++;
                }
            }
        }


        QString label = QString("Track %1 (notes: %2)").arg(i).arg(noteCount);
        ui->comboBox_tracks->addItem(label, i);
    }

    int comboIndex = ui->comboBox_tracks->findData(trackToSelect);
    if (comboIndex != -1)
    {
        ui->comboBox_tracks->setCurrentIndex(comboIndex);
    }
    else
    {
        if (ui->comboBox_tracks->count() > 0)
        {
            ui->comboBox_tracks->setCurrentIndex(0);
        }

    }

    ui->comboBox_tracks->blockSignals(false);


    int finalTrack = ui->comboBox_tracks->currentData().toInt();
    if (finalTrack >= 0 && finalTrack < currentMidi.getTrackCount())
    {
        drawPianoRoll(currentMidi, finalTrack);
        fillNoteTable(currentMidi, finalTrack);
    }


    if (currentMidi.getTrackCount() > 0)
    {
        ui->label_end->setText(QString::number(currentMidi.getFileDurationInSeconds(), 'f', 2));
    }
}

void MainWindow::saveMidi(const QString &outPath)
{
    currentMidi.write(outPath.toStdString());
}

void MainWindow::on_uploadButton_clicked()
{
    deleteTbtn_ShowEnab(0);

    path = QFileDialog::getOpenFileName(this, "Choose MIDI file", "","MIDI Files (*.mid *.midi)");

    if (path.isEmpty())
    {
        return;
    }

    currentMidi.clear();

    if (!currentMidi.read(path.toStdString()))
    {
        QMessageBox::warning(this, "Error", "Error reading MIDI file!");
        return;
    }

    currentMidi.doTimeAnalysis();
    currentMidi.linkNotePairs();

    ui->label_status->setText("Opened: " + QFileInfo(path).fileName());
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider->setMaximum(100);

    ui->comboBox_tracks->clear();

    if (currentMidi.getTrackCount() > 1)
    {
        deleteTbtn_ShowEnab(1);
    }

    for (int t = 0; t < currentMidi.getTrackCount(); t++)
    {
        int noteCount = 0;
        for (int i = 0; i < currentMidi[t].size(); i++)
        {
            if (currentMidi[t][i].isNoteOn())
            {
                noteCount++;
            }
        }
            ui->comboBox_tracks->addItem(
                QString("Track %1 (notes: %2)").arg(t).arg(noteCount), t);

    }

    ui->label_end->setText(QString::number(currentMidi.getFileDurationInSeconds(), 'f', 2));

    int noteTrack = findFirstNoteTrack(currentMidi);
    if (noteTrack < 0)
    {

        if (currentMidi.getTrackCount() > 0)
        {
            noteTrack = 0;
        }
        else
        {
            return;
        }
    }

    drawPianoRoll(currentMidi, noteTrack);
    fillNoteTable(currentMidi, noteTrack);



    ui->playButton->setEnabled(true);
    ui->comboBox_tracks->setCurrentIndex(0);

    MainWindow::updateTrackBoxes();
}

void MainWindow::fillNoteTable(const smf::MidiFile &midi, int track)
{

    QList<smf::MidiEvent*> notes;

    for (int i = 0; i < midi[track].size(); i++)
    {
        auto &ev = midi[track][i];

        if (ev.isNoteOn() && ev.isLinked())
        {
            notes.append(const_cast<smf::MidiEvent*>(&ev));
        }
    }

    ui->tableWidget_notes->setRowCount(notes.size());
    ui->tableWidget_notes->setColumnCount(4);
    ui->tableWidget_notes->setHorizontalHeaderLabels({"Pitch", "Start", "Duration", "Velocity"});

    for (int r = 0; r < notes.size(); r++)
    {
        auto *ev = notes[r];

        ui->tableWidget_notes->setItem(r, 0, new QTableWidgetItem(QString::number(ev->getKeyNumber())));
        ui->tableWidget_notes->setItem(r, 1, new QTableWidgetItem(QString::number(ev->seconds)));
        ui->tableWidget_notes->setItem(r, 2, new QTableWidgetItem(QString::number(ev->getDurationInSeconds())));
        ui->tableWidget_notes->setItem(r, 3, new QTableWidgetItem(QString::number(ev->getVelocity())));
    }
}

void MainWindow::drawPianoRoll(smf::MidiFile &midi, int track)
{
    scene->clear();


    const double scaleX = 150.0;
    const double scaleY = 12.0;
    const double noteHeight = 10.0;

    int minPitch = 127;
    int maxPitch = 0;


    for (int i = 0; i < midi[track].size(); i++)
    {
        auto &ev = midi[track][i];
        if (ev.isNoteOn() && ev.isLinked())
        {
            int p = ev.getKeyNumber();
            minPitch = qMin(minPitch, p);
            maxPitch = qMax(maxPitch, p);
        }
    }

    if (minPitch > maxPitch)
    {
        return;
    }



    for (int i = 0; i < midi[track].size(); i++)
    {
        auto &ev = midi[track][i];

        if (ev.isNoteOn() && ev.isLinked())
        {
            int pitch = ev.getKeyNumber();
            double start = ev.seconds;
            double dur = ev.getDurationInSeconds();


            double x = start * scaleX;
            double y = (maxPitch - pitch) * scaleY;

            QRectF rect(x, y, dur * scaleX, noteHeight);

            QColor color(0, 180, 255);
            color.setAlpha(200);

            scene->addRect(rect, QPen(Qt::black), QBrush(color));
        }
    }
}


void MainWindow::on_playButton_clicked()
{
    if (path.isEmpty())
    {
        return;
    }


    QString command = "open \"" + path + "\" type sequencer alias myMidi";
    mciSendStringW(reinterpret_cast<LPCWSTR>(command.utf16()), NULL, 0, NULL);
    mciSendStringW(L"play myMidi", NULL, 0, NULL);

    WCHAR buffer[128] = {0};
    mciSendStringW(L"status myMidi length", buffer, 128, NULL);

    int lengthMs = wcstol(buffer, nullptr, 10);

    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(lengthMs);
    ui->horizontalSlider->setValue(0);
    playbtn_ShowEnab(0);
    stopbtn_ShowEnab(1);
    timer->start(100);
}


void MainWindow::on_stopButton_clicked()
{
    mciSendStringW(L"stop myMidi", NULL, 0, NULL);
    mciSendStringW(L"close myMidi", NULL, 0, NULL);
    timer->stop();
    ui->horizontalSlider->setValue(0);
    stopbtn_ShowEnab(0);
    playbtn_ShowEnab(1);
}

void MainWindow::updateSlider()
{
    WCHAR buffer[128] = {0};
    mciSendStringW(L"status myMidi position", buffer, 128, NULL);

    int posMs = wcstol(buffer, nullptr, 10);
    ui->horizontalSlider->setValue(posMs);

    if (posMs >= ui->horizontalSlider->maximum())
    {
        timer->stop();
    }
}


void MainWindow::on_comboBox_tracks_currentIndexChanged(int index)
{
    if (index < 0)
    {
        return;
    }


    int track = ui->comboBox_tracks->currentData().toInt();

    drawPianoRoll(currentMidi, track);
    fillNoteTable(currentMidi, track);
}


void MainWindow::on_deleteTrackbtn_clicked()
{
    int track = ui->comboBox_tracks->currentData().toInt();
    currentMidi.deleteTrack(track);
    QMessageBox::warning(this, "Deleted", "Current track successfully deleted!");
    reloadMidi();
    saveButton_ShowEnab(1);

}


void MainWindow::on_saveButton_clicked()
{
    QString out = QFileDialog::getSaveFileName(this, "Save MIDI", "", "MIDI Files (*.mid)");

    if (out.isEmpty())
    {
        return;
    }
    saveMidi(out);
    saveButton_ShowEnab(0);
}


void MainWindow::on_addTrackbtn_clicked()
{

    int newTrackIndex = currentMidi.addTrack();

    smf::MidiEvent endTrackEv;
    endTrackEv.tick = 0;
    endTrackEv.track = newTrackIndex;
    currentMidi[newTrackIndex].push_back(endTrackEv);

    reloadMidi(newTrackIndex);
    saveButton_ShowEnab(true);
}


void MainWindow::on_mergeOpenbtn_clicked()
{

    if (ui->groupBoxMerge->isEnabled() && ui->groupBoxMerge->isEnabled())
    {
        ui->groupBoxMerge->setEnabled(false);
        ui->groupBoxMerge->setVisible(false);
    }
    else
    {
        ui->groupBoxMerge->setEnabled(true);
        ui->groupBoxMerge->setVisible(true);
    }

}

void MainWindow::updateTrackBoxes()
{
    ui->MergeBox_1->clear();
    ui->MergeBox_2->clear();
    ui->comboBox_tracks->clear();

    for (int t = 0; t < currentMidi.getTrackCount(); ++t)
    {
        int noteCount = 0;
        for (int i = 0; i < currentMidi[t].size(); ++i)
        {
            if (currentMidi[t][i].isNoteOn())
                noteCount++;
        }

        QString name = QString("Track %1 (notes: %2)").arg(t).arg(noteCount);

        ui->comboBox_tracks->addItem(name, t);
        ui->MergeBox_1->addItem(QString::number(t), t);
        ui->MergeBox_2->addItem(QString::number(t), t);
    }

    ui->comboBox_tracks->setCurrentIndex(0);
}


void MainWindow::on_mergeTrackbtn_clicked()
{
    int t1 = ui->MergeBox_1->currentData().toInt();
    int t2 = ui->MergeBox_2->currentData().toInt();

    if (t1 == t2)
    {
        QMessageBox::warning(this, "Error!", "You can't merge the same track!");
        return;
    }

    currentMidi.mergeTracks(t1, t2);

    reloadMidi();
    updateTrackBoxes();
    saveButton_ShowEnab(1);
}


