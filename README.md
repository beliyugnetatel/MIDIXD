# MIDIXD
## MIDIXD is a simple application that provides basic tools to work with a MIDI-file.
Program was made using QtCreator tool for C++ using Midifile.sapp library for parsing Standard MIDI Files and Windows MCI for playing sound.
### It allows user to:
-- [Open up a MIDI-file;](#opening-midi-file)\
-- [See file's note structure for chosen track via Piano-Roll or Note Table;](#note-structure)\
-- [Play uploaded file, including playback progressbar and pausing;](#playing-uploaded-file)\
-- [Add new empty tracks into the file structure;](#adding-new-track)\
-- [Delete chosen track;](#deleting-track)\
-- [Merge chosen tracks;](#merging-tracks)\
-- [Save changes.](#saving-changes)

## Opening MIDI-file
In order to upload a file from your computer, you have to press the "Upload" button:

<img width="617" height="419" alt="Untitled" src="https://github.com/user-attachments/assets/dc42a2b2-3d33-4d53-bba1-93e328bc963b" />

If everything is fine, after the file is uploaded, you can see the Note Structure and the name of the opened file near the "Upload" button, indicating success of the operation. At this stage, you can start working with the file.

## Note structure
When the file is loaded, you can see the structure of the selected track (by default it shows the very first one in the Piano-Roll).

<img width="624" height="418" alt="image" src="https://github.com/user-attachments/assets/a6a02c0b-b8ab-481e-8734-77b268e91f97" />

In the upper menu user can choose Note-Table that allows to see more detailed structure of the chosen track.

<img width="624" height="421" alt="image" src="https://github.com/user-attachments/assets/d1eef6b1-fa41-48af-b9fa-69afa3f5298a" />

This table shows note's pitch, velocity, start time and lenght.

## Playing uploaded file
Program allows user to play the uploaded file:

<img width="386" height="64" alt="image" src="https://github.com/user-attachments/assets/a3d0ff06-4702-402b-95f3-49e05be4fb6c" />

User can see the whole lenght of the file in munites and track the playback progress.

## Adding new track
It is possible to add a new empty track in the uploaded file's MIDI structure using the "Add New Track" button: 

<img width="974" height="662" alt="image" src="https://github.com/user-attachments/assets/506db411-6e87-4f11-bfe5-d3e3bea30cc2" />

## Deleting track
It is also possible to delete any track from the uploaded file's MIDI structure using the "Delete Track" button: 

<img width="974" height="664" alt="image" src="https://github.com/user-attachments/assets/0c61b0b2-738d-40c3-8aaf-4629eacdccf0" />

## Merging tracks
To merge tracks, select two tracks in the corresponding field and click the merge button. 

<img width="974" height="667" alt="image" src="https://github.com/user-attachments/assets/6c4b679b-6926-4e92-99a0-fdf4f5668b7c" />

However, if you select two identical tracks, a corresponding message will be displayed stating that the operation cannot be performed.

<img width="974" height="666" alt="image" src="https://github.com/user-attachments/assets/28ce3cd7-c592-4dbf-a96d-52a5654590bd" />

## Saving changes
After all the work is done user can download changed MIDI-file:
<img width="52" height="38" alt="image" src="https://github.com/user-attachments/assets/90e89395-fe56-42a5-956e-d9fa65a37b60" />

