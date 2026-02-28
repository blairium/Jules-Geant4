FROM physino/geant4:latest

# Install basic dependencies
# AlmaLinux uses dnf
RUN dnf update -y && dnf install -y \
    epel-release \
    && dnf install -y \
    git \
    cmake \
    gcc-c++ \
    make \
    wget \
    which \
    expat-devel \
    libX11-devel \
    libXmu-devel \
    mesa-libGL-devel \
    mesa-libGLU-devel \
    python3 \
    python3-pip \
    python3-devel \
    nlohmann-json-devel \
    && dnf clean all

# Create a virtual environment for Python
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

# Install python analysis tools in the virtual environment
RUN pip3 install --no-cache-dir numpy matplotlib uproot

# Install ROOT
# Using a pre-built binary for AlmaLinux 9
WORKDIR /opt
RUN wget https://root.cern/download/root_v6.30.02.Linux-almalinux9.3-x86_64-gcc11.4.tar.gz \
    && tar -xzf root_v6.30.02.Linux-almalinux9.3-x86_64-gcc11.4.tar.gz \
    && rm root_v6.30.02.Linux-almalinux9.3-x86_64-gcc11.4.tar.gz

# Source ROOT
ENV ROOTSYS=/opt/root
ENV PATH=$ROOTSYS/bin:$PATH
ENV LD_LIBRARY_PATH=$ROOTSYS/lib
ENV PYTHONPATH=$ROOTSYS/lib
ENV CMAKE_PREFIX_PATH=$ROOTSYS${CMAKE_PREFIX_PATH:+:${CMAKE_PREFIX_PATH}}

# Install Geant4 Datasets
# The base image has Geant4 libraries but not datasets to save space.
RUN geant4-config --install-datasets

# Copy the simulation code
COPY . /app/microelectronics_sim
WORKDIR /app/microelectronics_sim

# Build the simulation
# Rely on GEANT4 being in the path from the base image
# We remove the hardcoded Geant4_DIR since physino/geant4 configures it
RUN mkdir build \
    && cd build \
    && cmake .. \
    && make -j$(nproc)

# Default command
CMD ["/bin/bash"]
