FROM alpine:latest AS builder

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
gcc -o regex main.c $OBJ_FILES && \
echo "Linking test executable..." && \
gcc -o regex_test test/test.c $OBJ_FILES


FROM alpine

WORKDIR /app

COPY --from=builder /app/regex /app/
COPY --from=builder /app/regex_test /app/

ENTRYPOINT ["/app/regex"]
CMD []