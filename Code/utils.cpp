/**
 * @author Bruno Pena Baêta (696997)
 * @author Felipe Nepomuceno Coelho (689661)
 */

#include <sys/stat.h>
#include <string>

/**
 * @brief Checks if a file exists
 * 
 * @param name A string containing the file path
 * @return true if the file exists. false otherwise
 */
inline bool existsFile (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0); 
}