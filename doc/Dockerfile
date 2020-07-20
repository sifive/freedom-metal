FROM sphinxdoc/sphinx

RUN apt-get update \
 && apt install -y doxygen graphviz \
 && apt-get autoremove \
 && apt-get clean

RUN python3 -m pip install breathe pydot

WORKDIR /
