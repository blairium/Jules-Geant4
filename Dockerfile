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


ENV LANG=C.UTF-8

ARG ROOT_BIN=root_v6.36.08.Linux-almalinux9.7-x86_64-gcc11.5.tar.gz

WORKDIR /opt

COPY packages packages

RUN dnf update -q -y \
 && dnf install -y epel-release \
 && dnf install -y $(cat packages)\
 && rm -f packages \
 && curl -O https://root.cern/download/${ROOT_BIN} \
 && tar -xzvf ${ROOT_BIN} \
 && rm -f ${ROOT_BIN} \
 && echo /opt/root/lib >> /etc/ld.so.conf \
 && ldconfig

ENV ROOTSYS /opt/root
ENV PATH $ROOTSYS/bin:$PATH
ENV PYTHONPATH $ROOTSYS/lib:$PYTHONPATH
ENV CLING_STANDARD_PCH none



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
