# Geant4 Microelectronics Simulation

This repository contains a Geant4 simulation application designed to study the irradiation of a Silicon substrate with a Silicon Oxide (SiO2) overlayer by low-energy (185 eV) photons. The simulation focuses on the emission of secondary electrons and uses specific microelectronics physics models.

## Simulation Description

### Geometry
*   **Substrate**: 500 nm x 500 nm x 500 nm Silicon block.
*   **Overlayer**: 1 nm thick Silicon Oxide (SiO2) layer on top of the substrate.
*   **Vacuum**: Surrounding world volume.
*   **Detector**: A hemispherical sensitive boundary located 1000 nm above the sample surface to detect emitted electrons.

### Physics
The simulation utilizes a combination of standard and specialized physics lists:
*   **G4EmStandardPhysics_option4**: Handles standard electromagnetic interactions, including the primary photon interactions (e.g., Photoelectric effect).
*   **G4EmMicroElecPhysics**: Provides detailed low-energy electron transport models specifically for Silicon, essential for microdosimetry and secondary electron emission studies.

### Primary Source
*   **Particle**: Photon (gamma)
*   **Energy**: 185 eV
*   **Direction**: Normal incidence (perpendicular to the surface)
*   **Beam Profile**: Pencil beam with a diameter of 10 nm.

### Output
The simulation generates a ROOT file (`microelec_output.root`) containing:
*   **ElectronDistribution**: Ntuple with spatial (x, y, z) and energy information of electrons inside the Si and SiO2 volumes.
*   **DetectorHits**: Ntuple with position and energy of electrons reaching the detector boundary.
*   **EnergySpectrum**: Histogram of the kinetic energy of electrons at the detector.

## Running the Simulation

A `Dockerfile` is provided to set up the necessary environment (Geant4, ROOT, Python) and run the simulation.

### Prerequisites
*   Docker installed on your machine.

### Instructions

1.  **Build the Docker Image**:
    ```bash
    docker build -t geant4-microelec .
    ```

2.  **Run the Simulation**:
    ```bash
    docker run --rm -v $(pwd)/plots:/app/microelectronics_sim/plots geant4-microelec ./run_simulation.sh
    ```
    This command will:
    *   Compile the simulation code inside the container.
    *   Run the simulation for 1000 events.
    *   Execute the Python analysis script.
    *   Save the resulting plots to a `plots` directory in your current folder.

## Analysis and Results

The included Python script (`analyze.py`) processes the ROOT output file and generates the following plots:

1.  **electron_depth_profile.png**: Shows the distribution of electrons along the Z-axis (depth) inside the Silicon and SiO2 layers.
2.  **electron_lateral_distribution.png**: Shows the X-Y distribution of electron interactions inside the material.
3.  **detector_spatial_distribution.png**: Shows the X-Y position of electrons detected at the hemispherical boundary, colored by their kinetic energy.
4.  **secondary_electron_spectrum.png**: The energy spectrum of electrons emitted from the surface and reaching the detector. The binning is set to 0.05 eV steps from 0 to 100 eV.

### Example Output

After running the simulation, you will find `secondary_electron_spectrum.png` in the `plots` directory. This plot represents the number of electrons detected vs. their kinetic energy.
