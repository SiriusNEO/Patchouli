billFile.open(billFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
billFile.seekp(sizeof(int)+(tot-1)*sizeof(double), std::ios::beg);
billFile.write(reinterpret_cast<char *>(&bill[tot-1]), sizeof(double));
billFile.seekp(0, std::ios::beg);
billFile.write(reinterpret_cast<char *>(&tot), sizeof(int));
billFile.close();


