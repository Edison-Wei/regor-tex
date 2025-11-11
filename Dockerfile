FROM alpine AS builder

RUN apk add --no-cache build-base

WORKDIR /app

COPY . .

ENV CFLAGS="-Wall -Werror -Ilib/include -Ilib/util/include"

RUN echo "Starting compilation on Alpine..." && \

gcc $CFLAGS -c lib/match.c -o match.o && \
gcc $CFLAGS -c lib/parser.c -o parser.o && \
gcc $CFLAGS -c lib/util/util.c -o util.o && \

OBJ_FILES="match.o parser.o util.o" && \

echo "Linking main executable..." && \
gcc -o main main.c $OBJ_FILES && \

echo "Linking test executable..." && \
gcc -o test test/test.c $OBJ_FILES


FROM alpine:latest

WORKDIR /app

COPY --from=builder /app/main /app/
COPY --from=builder /app/test /app/

ENTRYPOINT ["/app/main"]
CMD []