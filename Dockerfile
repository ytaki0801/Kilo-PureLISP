FROM debian:stable-slim
RUN apt update
RUN apt install -y git gcc
RUN git clone https://github.com/ytaki0801/Kilo-PureLISP.git
WORKDIR /Kilo-PureLISP
RUN cc -static -o kplisp -DPURE_LISP_LIB kilo-BYOTE130.c PureLISP.c kplisp.c

FROM busybox
WORKDIR /
COPY --from=0 /Kilo-PureLISP/kplisp .
COPY --from=0 /Kilo-PureLISP/sample.plsh .
CMD ["./kplisp", "sample.plsh"]
