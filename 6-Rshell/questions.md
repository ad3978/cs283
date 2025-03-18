1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The client uses a special character to mark the end of a message. When receiving data, the client reads incoming information and looks for this specific character. This method helps manage different message lengths and network transmission challenges. The client collects data in pieces and uses the eof character to know when the entire message has been received. This ensures that the client can handle messages of varying sizes and know exactly when the transmission is complete

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Network communication is complex because data does not always arrive in a predictable way. When sending information, the data can be split into smaller pieces called packets. These packets might arrive out of order or at different times. Computer programs must create methods to reassemble these packets correctly and ensure the entire message makes sense. Without careful management, messages can become jumbled or incomplete, making it difficult to understand the original communication.

3. Describe the general differences between stateful and stateless protocols.

Stateful communication basically remembers information from previous interactions. The server keeps track of past communication context which allows for more complex interactions but requires more resources. Stateless communication treats each message as a completely new and independent interaction with no memory of previous exchanges. Stateful protocols like TCP maintain connection information, while stateless protocols like HTTP treat each request as a fresh start. The choice depends on the specific use case scenarios

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

Some network applications prioritize speed over perfect data transmission. UDP  is faster and uses less network resources compared to more reliable protocols. It works well for applications where small data losses are acceptable such as online gaming and video streaming. In these scenarios a slight loss of information is preferable to significant delays in transmission. UDP provides quick communication with minimal overhead making it suitable for such time sensitive network interactions

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Computers use a software interface called the socket API to communicate over networks. This API provides a set of standard functions that allow programs to create network connections, send and receive data, and close connections. The operating system manages these sockets providing a consistent way for different programs to communicate across various network types
