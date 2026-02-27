FROM ubuntu:22.04

# Install basic dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    libexpat1-dev \
    libx11-dev \
    libxmu-dev \
    python3 \
    python3-pip \
    python3-venv \
    && rm -rf /var/lib/apt/lists/*

# Create a virtual environment for Python
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

# Install python analysis tools in the virtual environment
RUN pip3 install --no-cache-dir numpy matplotlib uproot

# Install ROOT
# Using a pre-built binary for Ubuntu 22.04
WORKDIR /opt
RUN wget https://root.cern/download/root_v6.30.02.Linux-ubuntu22.04-x86_64-gcc11.4.tar.gz \
    && tar -xzf root_v6.30.02.Linux-ubuntu22.04-x86_64-gcc11.4.tar.gz \
    && rm root_v6.30.02.Linux-ubuntu22.04-x86_64-gcc11.4.tar.gz

# Source ROOT
ENV ROOTSYS=/opt/root
ENV PATH=$ROOTSYS/bin:$PATH
ENV LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
ENV PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH

# Install Geant4
# We will build Geant4 from source as it's often the most reliable way to get specific configurations
# and datasets.
WORKDIR /opt
RUN git clone -b v11.2.0 --depth 1 https://github.com/Geant4/geant4.git geant4_src \
    && mkdir geant4_build \
    && cd geant4_build \
    && cmake -DCMAKE_INSTALL_PREFIX=/opt/geant4 \
             -DGEANT4_INSTALL_DATA=ON \
             -DGEANT4_USE_OPENGL_X11=ON \
             -DGEANT4_USE_RAYTRACER_X11=ON \
             -DGEANT4_BUILD_MULTITHREADED=ON \
             ../geant4_src \
    && make -j$(nproc) \
    && make install \
    && cd .. \
    && rm -rf geant4_src geant4_build

# Environment variables for Geant4
ENV GEANT4_DIR=/opt/geant4
ENV PATH=$GEANT4_DIR/bin:$PATH
ENV LD_LIBRARY_PATH=$GEANT4_DIR/lib:$LD_LIBRARY_PATH

# Copy the simulation code
COPY . /app/microelectronics_sim
WORKDIR /app/microelectronics_sim

# Build the simulation
# Rely on GEANT4_DIR being set and CMAKE finding the config files automatically
# or point to the standard cmake directory.
RUN mkdir build \
    && cd build \
    && cmake -DGeant4_DIR=$GEANT4_DIR/lib/cmake/Geant4 .. \
    && make -j$(nproc)

# Default command
CMD ["/bin/bash"]
