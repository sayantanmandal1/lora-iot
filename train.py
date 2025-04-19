import joblib
model = joblib.load("ml_model.pkl")
print("Healthy Ranges:", model.feature_range_)
