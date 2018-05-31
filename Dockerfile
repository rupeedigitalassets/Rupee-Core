
FROM ubuntu:16.04

MAINTAINER Ashish Srivastava <me@ashishsrivastav.com>

ENV SRC_DIR /usr/local/src/rupeed
ENV CONF_DIR /root/.RupeeCore/

ADD Rupee.conf $CONF_DIR

RUN set -x \
  && buildDeps=' \
      ca-certificates \
      cmake \
      g++ \
      git \
      libboost1.58-all-dev \
      libssl-dev \
      make \
      pkg-config \
  ' \
  && apt-get -qq update \
  && apt-get -qq --no-install-recommends install $buildDeps

  RUN apt-get install qt5-default qt5-qmake qtbase5-dev-tools qttools5-dev-tools \
    build-essential libboost-dev libboost-system-dev \
    libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev \
    libssl-dev libdb++-dev libminiupnpc-dev -y \
    dh-autoreconf -y \
    libgmp3-dev -y \
    


RUN git clone https://github.com/rupeedigitalassets/Rupee-Core.git $SRC_DIR

WORKDIR $SRC_DIR/src

RUN chmod +x secp256k1/autogen.sh

RUN make -f makefile.unix

RUN ls

EXPOSE 8518
EXPOSE 8517

CMD ./Rupeed
