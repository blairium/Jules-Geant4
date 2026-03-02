FROM physino/geant4:11.4.0

ENV LANG=C.UTF-8

ARG ROOT_BIN=root_v6.36.08.Linux-almalinux9.7-x86_64-gcc11.5.tar.gz

WORKDIR /opt

COPY packages packages
# Update packages & Install ROOT
RUN dnf update -q -y \
 && dnf install -y yum-utils \
 && dnf config-manager --set-enabled crb \
 && dnf install -y epel-release \
 && dnf install --allowerasing -y $(cat packages)\
 && rm -f packages \
 && curl -O https://root.cern/download/${ROOT_BIN} \
 && tar -xzvf ${ROOT_BIN} \
 && rm -f ${ROOT_BIN} \
 && echo /opt/root/lib >> /etc/ld.so.conf \
 && ldconfig 


# Create a virtual environment for Python
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"




ENV ROOTSYS=/opt/root
ENV PATH=$ROOTSYS/bin:$PATH
ENV PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH
ENV CLING_STANDARD_PCH=none


# Install python analysis tools in the virtual environment
RUN pip3 install --no-cache-dir numpy matplotlib uproot


# Copy the simulation code
COPY . /app/microelectronics_sim
WORKDIR /app/microelectronics_sim


# Default command
CMD ["/bin/bash"]
