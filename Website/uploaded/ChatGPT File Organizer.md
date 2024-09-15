
# File Organizer Script

This Python script automatically organizes downloaded files into specific folders based on their file types. It watches for new files in the download directory and moves them to appropriate folders such as PDFs, Images, Audio, and Video files.

## Requirements

- Python 3
- `watchdog` library

## Installation

First, ensure that Python 3 is installed on your system. If not, install it from [python.org](https://www.python.org/downloads/).

Then, install the `watchdog` library which is required to monitor directory changes. You can install it using pip:

```bash
pip install watchdog
```

## Configuration

Edit the script to set up your desired source and destination directories. By default, the source directory is set to `/Users/user/Downloads`. You can modify the following lines to fit your directory structure:

```python
destination_pdf = "/path/to/PDF"
destination_img = "/path/to/IMG"
destination_audio = "/path/to/AUDIO"
destination_video = "/path/to/VIDEO"
source_dir = "/path/to/Downloads"
```

## Running the Script

To run the script, navigate to the directory containing the script and run:

```bash
python file_organizer.py
```

The script will start monitoring the specified download directory. Any new files added there will be automatically moved to the appropriate directory based on their file type.

## Stopping the Script

To stop the script, use `CTRL+C` in the terminal.
