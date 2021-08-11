import yaml
import sys
# from ptk.logging import logerror

class ConfigLoader():
    """[Wrapper to load, write and read from yaml config files.]
    """
    def __init__(self, file_path):
        self._file_path = file_path
        self._config_dictionary = None
        try:
            with open(self._file_path, "r") as file:
                self._config_dictionary = yaml.safe_load(file)
        except Exception as e:
            print("Failure to load config from file {}: {}".format(self._file_path, str(e)))
            sys.exit(0)

    def __repr__(self) -> str:
        return str(self._config_dictionary)

    @property
    def config(self):
        """[Property to get the configuration dict]

        Returns:
            [dict]: [description]
        """
        return self._config_dictionary

    def getValue(self, key):
        """[Gets the value from the dictionary with the specified key if it
        exists. Otherwise returns None.]

        Args:
            key ([Any]): [Key to use]

        Returns:
            [Any]: [Value retrieved from the configuration dict if exists, else None]
        """
        if key in self._config_dictionary:
            return self._config_dictionary[key]
        return None

    def write_full_config(self, dictionary):
        #untested
        with open(self._file_path, "w") as file:
            yaml.safe_dump_all(dictionary)
