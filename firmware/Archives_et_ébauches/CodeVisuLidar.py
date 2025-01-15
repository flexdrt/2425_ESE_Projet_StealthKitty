import serial
import math
import numpy as np
from sklearn.cluster import DBSCAN
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt

PORT = 'COM18'
BAUDRATE = 115200
TIMEOUT = 1.0

ser = serial.Serial(PORT, BAUDRATE, timeout=TIMEOUT)

# Configuration matplotlib
plt.ion()
fig = plt.figure(figsize=(6, 6))
ax = fig.add_subplot(111)
ax.set_xlim([-3000, 3000])
ax.set_ylim([-3000, 3000])
ax.set_aspect('equal', 'box')
ax.grid(True)
plt.title("Vue Sonar LIDAR + Clustering")

angles = []
distances = []
last_angle = None
last_distance = None

try:
    while True:
        ligne = ser.readline().decode().strip()
        
        if not ligne:
            continue

        if ligne == '#':
            if angles and distances:
                # Conversion coordonnées polaires vers cartésiennes
                angles_rad = np.radians(angles)
                xs = np.array(distances) * np.cos(angles_rad)
                ys = np.array(distances) * np.sin(angles_rad)

                # Clustering DBSCAN
                coords = np.column_stack((xs, ys))
                db = DBSCAN(eps=100, min_samples=3).fit(coords)
                labels = db.labels_

                # Ne garder que les points appartenant à un cluster (suppression des outliers)
                mask = labels != -1
                xs = xs[mask]
                ys = ys[mask]
                labels = labels[mask]

                unique_labels = set(labels)
                nb_clusters = len(unique_labels)
                print(f"Nombre de clusters détectés : {nb_clusters}")

                # Rafraîchissement affichage
                ax.clear()
                ax.set_xlim([-3000, 3000])
                ax.set_ylim([-3000, 3000])
                ax.set_aspect('equal', 'box')
                ax.grid(True)
                ax.set_title("Vue Sonar LIDAR + Clustering")

                # Affichage clusters
                cmap = plt.get_cmap('rainbow')
                colors = [cmap(lbl / nb_clusters) for lbl in labels]
                ax.scatter(xs, ys, c=colors, s=15)

                # Affichage point cible
                if last_angle is not None and last_distance is not None and last_distance > 0:
                    x_cible = last_distance * math.cos(math.radians(last_angle))
                    y_cible = last_distance * math.sin(math.radians(last_angle))
                    ax.scatter([x_cible], [y_cible], c='red', s=50, marker='o')

                fig.canvas.draw()
                fig.canvas.flush_events()

                angles.clear()
                distances.clear()
            continue

        else:
            try:
                angle_str, dist_str = ligne.split(',')
                angle = float(angle_str.strip())
                distance = float(dist_str.strip())
                
                # Ignorer les valeurs à distance 0
                if distance > 0:
                    angles.append(angle)
                    distances.append(distance)
                    last_angle = angle
                    last_distance = distance

            except ValueError:
                continue

except KeyboardInterrupt:
    print("Arrêt par l'utilisateur.")

finally:
    ser.close()
    print("Port série fermé.")