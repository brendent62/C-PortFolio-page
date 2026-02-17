FROM frolvlad/alpine-gcc:latest

WORKDIR /app

COPY server.c ./

RUN gcc server.c -o server

EXPOSE 8000

CMD ["./server"]