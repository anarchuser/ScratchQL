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
    google::InitGoogleLogging(argv [0]);

    LOG (INFO) << "Start Running";
    LOG (INFO) << PROJECT_ROOT;

    capnp::EzRpcServer server (kj::heap <DatabaseImpl <DBMS>>(), "*");
    uint port = server.getPort().wait (server.getWaitScope ());
    std::cout << ADDRESS << ":" << port << std::endl;

    Client client (ADDRESS, port);

    client.startInterface([] (Table const & t) {
        std::cout << "Received Query!" << std::endl;
    });

    LOG (INFO) << "Stop Running";
}
