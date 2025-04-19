import requests
import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
import joblib

THINGSPEAK_API_KEY = "I7R88EYL2VLK5S8B"
THINGSPEAK_CHANNEL_ID = "2859742"
THINGSPEAK_URL = f"https://api.thingspeak.com/channels/{THINGSPEAK_CHANNEL_ID}/feeds.csv?api_key={THINGSPEAK_API_KEY}&results=1000"

def fetch_data():
    response = requests.get(THINGSPEAK_URL)
    if response.status_code == 200:
        with open("thingspeak_data.csv", "w") as f:
            f.write(response.text)
        print("Data downloaded successfully.")
    else:
        print("Failed to fetch data.")

def train_model():
    df = pd.read_csv("thingspeak_data.csv")
    df = df.dropna()
    df = df[['field1', 'field2', 'field3']]
    df.columns = ['temperature', 'humidity', 'moisture']
    
    # Define unhealthy conditions based on the collected dataset
    df['label'] = ((df['temperature'] < 15) | (df['temperature'] > 35) | 
                   (df['humidity'] < 30) | (df['humidity'] > 80) | 
                   (df['moisture'] < 300) | (df['moisture'] > 800)).astype(int)

    X = df[['temperature', 'humidity', 'moisture']]
    y = df['label']
    
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
    model = RandomForestClassifier(n_estimators=100, random_state=42)
    model.fit(X_train, y_train)
    
    print("Model trained successfully.")
    joblib.dump(model, "ml_model.pkl")
    
    thresholds = {
        "temperature": (df["temperature"].quantile(0.05), df["temperature"].quantile(0.95)),
        "humidity": (df["humidity"].quantile(0.05), df["humidity"].quantile(0.95)),
        "moisture": (df["moisture"].quantile(0.05), df["moisture"].quantile(0.95))
    }

    print("Temperature Healthy Range:", thresholds["temperature"])
    print("Humidity Healthy Range:", thresholds["humidity"])
    print("Soil Moisture Healthy Range:", thresholds["moisture"])
    
    joblib.dump(thresholds, "thresholds.pkl")
    print("Thresholds saved as thresholds.pkl")

if __name__ == "__main__":
    fetch_data()
    train_model()
