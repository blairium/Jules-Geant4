import uproot
import matplotlib.pyplot as plt
import numpy as np
import os

# Output directory for plots
output_dir = "plots"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Read the ROOT file
# The run action saves to "microelec_output.root" (or similar, depending on multithreading)
# If multithreading is on, it might be microelec_output_t0.root, etc.
# We will assume single threaded or merged output for simplicity in this script.
filename = "microelec_output.root"
if not os.path.exists(filename):
    # Try finding files matching pattern
    files = [f for f in os.listdir('.') if f.startswith('microelec_output') and f.endswith('.root')]
    if files:
        filename = files[0] # Take the first one found
    else:
        print("Error: ROOT output file not found.")
        exit(1)

print(f"Analyzing {filename}...")

try:
    file = uproot.open(filename)
except Exception as e:
    print(f"Error opening ROOT file: {e}")
    exit(1)

# 1. Spatial Distribution of Electrons inside Si and SiO2
try:
    tree_dist = file["ElectronDistribution"]
    x = tree_dist["x"].array(library="np")
    y = tree_dist["y"].array(library="np")
    z = tree_dist["z"].array(library="np")
    volID = tree_dist["volumeID"].array(library="np")

    # Plot Z distribution (depth profile)
    plt.figure(figsize=(10, 6))
    plt.hist(z[volID==0], bins=100, alpha=0.7, label='Silicon (Substrate)', color='blue')
    plt.hist(z[volID==1], bins=100, alpha=0.7, label='SiO2 (Overlayer)', color='green')
    plt.xlabel('Z Position (nm)')
    plt.ylabel('Count')
    plt.title('Depth Distribution of Electrons')
    plt.legend()
    plt.grid(True)
    plt.savefig(f"{output_dir}/electron_depth_profile.png")
    plt.close()

    # Plot X-Y distribution (top view)
    plt.figure(figsize=(8, 8))
    plt.scatter(x, y, s=1, alpha=0.1)
    plt.xlabel('X Position (nm)')
    plt.ylabel('Y Position (nm)')
    plt.title('Lateral Distribution of Electrons')
    plt.axis('equal')
    plt.grid(True)
    plt.savefig(f"{output_dir}/electron_lateral_distribution.png")
    plt.close()

except Exception as e:
    print(f"Could not analyze spatial distribution: {e}")

# 2. Detector Analysis
try:
    tree_det = file["DetectorHits"]
    x_det = tree_det["x"].array(library="np")
    y_det = tree_det["y"].array(library="np")
    z_det = tree_det["z"].array(library="np")
    ke_det = tree_det["kineticEnergy"].array(library="np")

    # Spatial Distribution at Detector
    plt.figure(figsize=(8, 8))
    plt.scatter(x_det, y_det, c=ke_det, cmap='viridis', s=5)
    plt.colorbar(label='Kinetic Energy (eV)')
    plt.xlabel('X Position (nm)')
    plt.ylabel('Y Position (nm)')
    plt.title('Electron Distribution at Detector')
    plt.axis('equal')
    plt.grid(True)
    plt.savefig(f"{output_dir}/detector_spatial_distribution.png")
    plt.close()

    # Energy Spectrum
    # Using the Ntuple data to recreate the spectrum for flexible plotting
    plt.figure(figsize=(10, 6))
    # Binning: 0 to 100 eV with 0.05 eV steps
    bins = np.arange(0, 100.05, 0.05)
    plt.hist(ke_det, bins=bins, color='red', alpha=0.7, edgecolor='black', linewidth=0.5)
    plt.xlabel('Kinetic Energy (eV)')
    plt.ylabel('Count')
    plt.title('Secondary Electron Emission Spectrum')
    plt.xlim(0, 100)
    plt.grid(True)
    plt.savefig(f"{output_dir}/secondary_electron_spectrum.png")
    plt.close()

    print("Analysis complete. Plots saved to 'plots' directory.")

except Exception as e:
    print(f"Could not analyze detector hits: {e}")
