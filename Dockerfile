FROM physino/geant4:11.4.0

# Install basic dependencies
# AlmaLinux uses dnf
RUN dnf install -y epel-release dnf-plugins-core && dnf config-manager --set-enabled crb && dnf update -y && dnf install -y \
    avahi-compat-libdns_sd-devel \
    avahi-devel \
    binutils \
    curl-minimal --allowerasing \
    davix-devel \
    fcgi \
    fcgi-devel \
    fftw-devel \
    ftgl-devel \
    gcc \
    gcc-c++ \
    gcc-gfortran \
    giflib-devel \
    git \
    gl2ps-devel \
    gl2ps-devel \
    glew-devel \
    graphviz-devel \
    gsl-devel \
    libAfterImage-devel \
    libjpeg-turbo-devel \
    libtiff-devel \
    libX11-devel \
    libXext-devel \
    libXft-devel \
    libxml2-devel \
    libXpm-devel \
    libzstd-devel \
    lz4-devel \
    make \
    mesa-libGL-devel \
    mesa-libGLU-devel \
    ncurses-libs \
    ocaml-findlib-devel \
    openssl-devel \
    pcre-devel \
    protobuf-compiler \
    protobuf-devel \
    pythia8-devel \
    python3 \
    python3-devel \
    python3-numpy \
    python3-pip \
    qt5-qtwebengine-devel \
    rapidyaml-devel \
    redhat-rpm-config \
    tbb tbb-devel \
    unuran-devel \
    xrootd-server-devel \
    xxhash-libs xxhash-devel \
    xz-devel \
    zeromq-devel \
    zlib-devel \
    cmake \
    wget \
    which \
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
RUN wget https://root.cern/download/root_v6.36.08.Linux-almalinux9.7-x86_64-gcc11.5.tar.gz \
    && tar -xzf root_v6.36.08.Linux-almalinux9.7-x86_64-gcc11.5.tar.gz \
    && rm root_v6.36.08.Linux-almalinux9.7-x86_64-gcc11.5.tar.gz

# Source ROOT
ENV ROOTSYS=/opt/root
ENV PATH=$ROOTSYS/bin:$PATH
ENV LD_LIBRARY_PATH=$ROOTSYS/lib
ENV PYTHONPATH=$ROOTSYS/lib

# Install Geant4 Datasets
# The base image has Geant4 libraries but not datasets to save space.
RUN geant4-config --install-datasets

# Copy the simulation code
COPY . /app/microelectronics_sim
WORKDIR /app/microelectronics_sim


# Default command
CMD ["/bin/bash"]
