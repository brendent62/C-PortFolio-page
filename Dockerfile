FROM gcc:alpine

WORKDIR /app

COPY server.c ./

RUN gcc server.c -o server

EXPOSE 22556

CMD ["./server""]