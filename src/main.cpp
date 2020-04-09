/* Copyright (C) 2020 Aaron Alef <aaron.alef@code.berlin>
 *                  & Felix Bachstein <felix.bachstein@code.berlin>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (e.g., ./LICENSE).
 * If not, see <https://www.gnu.org/licenses/>.
 */

#include "main.h"

int main (int argc, char * argv[]) {
    google::InitGoogleLogging (argv[0]);

    LOG (INFO) << "Start Running";
    LOG (INFO) << PROJECT_ROOT;

    std::string address (argc > 2 ? argv[2] : STR+ "*:" + std::to_string (PORT));
    auto execServer = [] (std::string const & address, bool never_done = false) -> capnp::EzRpcServer const & {
        (std::cout << "Setting database up on '" << address << "'...").flush();
        capnp::EzRpcServer server (kj::heap <DatabaseImpl <DBMS>> (), address);
        server.getPort().wait (server.getWaitScope());
        std::cout << " Done" << std::endl;

        if (never_done) kj::NEVER_DONE.wait (server.getWaitScope ());
        return std::move (server);
    };

    auto execClient = [] (std::string const & address, int port = -1) {
        Client client = (port < 0) ? Client (address) : Client (address, port);
        client.startInterface ([] (Response r) {std::cout << r;});
    };

    if (argc == 1) {
        auto const & server = execServer (address);
        execClient (ADDRESS, PORT);
    } else {
        std::string param (argv [1]);
        if (param == "--server") {
            auto const & server = execServer (address, true);
        }
        else if (param == "--client") {
            if (argc > 2) {
                execClient (address);
            } else {
                execClient (ADDRESS, PORT);
            }
        }
        else {
            std::cerr << "Unknown parameters: " << param << std::endl;
            std::cerr << "Syntax: " << argv [0] << " (--server|--client) (address:port)";
            return EXIT_FAILURE;
        }
    }

    LOG (INFO) << "Stop Running";
}
