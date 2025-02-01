#include <gtest/gtest.h>
#include <emscripten.h>
#include <dirent.h> // Pour opendir, readdir, closedir
#include <cstdio>   // pour fopen et fclose

// void listFiles(const std::string &path)
// {
//     DIR *dir = opendir(path.c_str());
//     if (!dir)
//     {
//         std::cerr << "Impossible d'ouvrir le dossier : " << path << std::endl;
//         return;
//     }

// struct dirent *entry;
// while ((entry = readdir(dir)) != NULL)
// {
//     std::cout << "Fichier trouvé : " << entry->d_name << std::endl;
// }

// closedir(dir);
// }

// void copy_file_from_memfs_to_local()
// {
//     // EM_ASM is a macro to call in-line JavaScript code.
//     EM_ASM(
//         // Make a directory other than '/'
//         FS.mkdir('/offline');
//         // Then mount with IDBFS type
//         FS.mount(IDBFS, {}, '/offline');

// // Then sync
// FS.syncfs(true, function(err) {
//                     // Error
//                 });
// );

// FILE *off_file = fopen("test_core_report.xml", "wb");

// if (off_file == nullptr)
// { // Vérifier si l'ouverture a échoué
//     perror("Erreur d'ouverture du fichier");
//     return;
// }

// EM_ASM(
//     FS.syncfs(function(err) {
//         perror("FS.syncfs");
//     });
// );

// fclose(off_file); // Fermer le fichier après usage
// }

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    // std::cout << "Liste des fichiers créés dans MEMFS :" << std::endl;
    // listFiles("/");    // Lister les fichiers à la racine
    // listFiles("/tmp"); // Tester si des fichiers sont écrits dans /tmp
    // copy_file_from_memfs_to_local();
    return result;
}