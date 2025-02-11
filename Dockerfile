FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /project

RUN apt-get update && apt-get install -y \
valgrind \
nano \
lldb \
build-essential \
sudo \
curl \
ncurses-base \
wget \
git

# LLDB: disable aslr
RUN echo "settings set target.disable-aslr false" >> ~/.lldbinit

# Create user 'user'
RUN useradd -m -s /bin/bash user \
&& usermod -aG sudo user \
&& echo 'user ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/user

USER user