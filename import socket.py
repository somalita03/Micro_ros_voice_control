import requests
import pyttsx3  # For feedback through speech
import speech_recognition as sr

# Replace with the IP address of your ESP32
ESP32_IP = "http://192.168.50.23"

# Initialize text-to-speech engine
engine = pyttsx3.init()

def send_signal(signal):
    try:
        if signal == "1":
            url = f"{ESP32_IP}/GPIO=1"
        elif signal == "2":
            url = f"{ESP32_IP}/GPIO=2"
        else:
            url = f"{ESP32_IP}/GPIO=0"  # Default case for unrecognized signal
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Signal {signal} sent successfully.")
            engine.say(f"Signal {signal} sent successfully.")
        else:
            print(f"Failed to send signal {signal}: {response.status_code}")
            engine.say(f"Failed to send signal {signal}.")
        engine.runAndWait()
    except Exception as e:
        print(f"Error sending signal {signal}: {e}")
        engine.say(f"Error sending signal {signal}.")
        engine.runAndWait()

def recognize_speech():
    recognizer = sr.Recognizer()
    microphone = sr.Microphone()

    print("Listening for commands... (say 'go' or 'stop')")

    while True:
        try:
            with microphone as source:
                recognizer.adjust_for_ambient_noise(source)  # Adjust to background noise
                audio = recognizer.listen(source)           # Listen for audio input

            # Recognize speech using Google Web Speech API (online)
            command = recognizer.recognize_google(audio).lower()

            if "go" in command:
                print("Command recognized: go")
                send_signal("1")
            elif "stop" in command:
                print("Command recognized: stop")
                send_signal("2")
            elif "quit" in command or "exit" in command:
                print("Exiting...")
                engine.say("Exiting the program.")
                engine.runAndWait()
                break
            else:
                print(f"Unrecognized command: {command}")
                engine.say(f"Unrecognized command: {command}")
                engine.runAndWait()
        except sr.UnknownValueError:
            print("Could not understand the audio. Please try again.")
            engine.say("Could not understand the audio. Please try again.")
            engine.runAndWait()
        except sr.RequestError as e:
            print(f"Could not request results from the speech recognition service; {e}")
            engine.say("There was an error with the speech recognition service.")
            engine.runAndWait()

def main():
    print("Speak 'go' to send signal 1 (GPIO_1 and GPIO_2 HIGH)")
    print("Speak 'stop' to send signal 2 (GPIO_3 and GPIO_4 HIGH)")
    print("Speak 'quit' or 'exit' to end the program.")
    recognize_speech()

if __name__ == "__main__":
    main()
