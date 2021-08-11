import re

def _reformat_directory(path):
    """[Reformates a directory path in the case where multiple "/"'s were appended to the path]

    Args:
        path ([str]): [Path]

    Returns:
        [str]: [Reformated path]
    """
    return re.sub('/+','/', path)