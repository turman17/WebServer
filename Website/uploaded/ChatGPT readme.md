# WeatherBot

WeatherBot is a Telegram bot that provides real-time weather updates for any specified city. The bot interacts with users via text commands, offering detailed weather information including temperature, wind speed, humidity, and sunrise/sunset times. It utilizes the OpenWeatherMap API to fetch weather data.

## Features

- Real-time weather updates by city
- Current temperature (in Celsius and Fahrenheit)
- Wind speed and humidity levels
- Weather condition descriptions
- Sunrise and sunset times

## Prerequisites

Before running the bot, you need to have Python installed on your machine. Additionally, you need to obtain an API key from OpenWeatherMap by signing up on their website.

## Installation

1. Clone the repository:
   ```
   git clone <repository-url>
   ```

2. Install the required Python packages:
   ```
   pip install requests python-telegram-bot
   ```

3. Enter your OpenWeatherMap API key in `weather.py`:
   ```python
   API_KEY = "YOUR_API_KEY"
   ```

4. Set your Telegram Bot token in `telegram_bot.py`:
   ```python
   TOKEN = "YOUR_TELEGRAM_BOT_TOKEN"
   ```

## Usage

Run the Telegram bot by executing:
```
python telegram_bot.py
```

Interact with the bot on Telegram using the following commands:
- `/start` to initiate interaction
- `/help` for information on available commands
- `/weather <city>` to get weather updates for the specified city

## Code Structure

- `weather.py`: Contains functions to interact with the OpenWeatherMap API and process weather data.
- `telegram_bot.py`: Handles bot commands and messages, interfacing with the Telegram API.

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests with your suggested changes.

## License

Specify your license or leave this as "MIT License" by default.