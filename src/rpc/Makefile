GEN_SRC := ProxyWebService.cpp proxy_web_service_constants.cpp proxy_web_service_types.cpp
GEN_OBJ := $(patsubst %.cpp,%.o, $(GEN_SRC))

SERVER=web_server
CLIENT=web_client

CXXFLAGS = -std=c++11 -Wall

.PHONY: all clean

all: $(SERVER) $(CLIENT)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H  -c $< -o $@

$(SERVER): $(SERVER:=.o) $(GEN_OBJ)
	$(CXX) $^ -o $@  -lthrift -lcurl
#$(POLICY:.cpp=.o)

$(CLIENT): $(CLIENT:=.o) $(GEN_OBJ)
	$(CXX) $^ -o $@  -lthrift 

clean:
	$(RM) *.o $(CLIENT) $(SERVER)

