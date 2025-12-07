import random
import csv

def generate_dataset(filename, num_records):
    """
    Generate random IoT sensor data and save to CSV file.

    Args:
        filename (str): Output CSV file name
        num_records (int): Number of records to generate
    """

    # Define CSV columns
    columns = ['temperature', 'humidity', 'soil_moisture', 'aqi_sensor', 'pump_state']

    # Open CSV for writing
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(columns)

        for _ in range(num_records):
            # Generate random sensor data
            temperature = round(random.uniform(0, 60), 2)  # °C
            humidity = round(random.uniform(0, 100), 2)    # %
            soil_moisture = random.choice([0, 1])          # 0 = dry, 1 = moist
            aqi_sensor = round(random.uniform(2, 999), 2) 

            pump_state = "OFF"
            # Determine pump state based on conditions
            if temperature > 49 or humidity < 10 or soil_moisture == 0:
                pump_state = "ON"
            
            soil_stat = "DRY"
            if soil_moisture == 1:
                soil_stat = "WET"
            

            # Write record
            writer.writerow([temperature, humidity, soil_stat, aqi_sensor, pump_state])

    print(f"✅ Dataset '{filename}' created successfully with {num_records} records.\n")

    # Analyze dataset: calculate % of 0s and 1s for binary columns
    analyze_binary_columns(filename, ['humidity','soil_moisture',  'pump_state'])


def analyze_binary_columns(filename, binary_columns):
    """Analyze and print percentage of 0 and 1 states in given columns."""
    import pandas as pd

    df = pd.read_csv(filename)
    print("📊 Binary Sensor State Distribution (%):\n")
    for col in binary_columns:
        counts = df[col].value_counts(normalize=True) * 100
        zero_pct = round(counts.get(0, 0), 2)
        one_pct = round(counts.get(1, 0), 2)
        print(f"{col}: 0 → {zero_pct}% , 1 → {one_pct}%")


# Example usage
if __name__ == "__main__":
    generate_dataset("sensor_data.csv", 50000)
