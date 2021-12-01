# Replace illegal characters with specified characters, with a definition of
# illegal characters as all characters other than those specified by the
# argument legal_chars.

replaceIllegalChars <- function(string, illegal_chars=NULL, legal_chars=NULL, repl_chars=NULL, compress_space=FALSE)
{
    # For example, a definition of legal characters for state (drug) names
    # used in experiment design can be:
    # legal_chars <- "-0-9A-Za-z_\\+"
    # Then all characters other than the defined legal characters will be
    # replaced by repl_chars.

    # Verify illegal_chars is a non-empty string.
    if(!is.null(illegal_chars)) stopifnot(is.character(illegal_chars) && nchar(illegal_chars)>0)
    # Verify legal_chars is a non-empty string.
    if(!is.null(legal_chars)) stopifnot(is.character(legal_chars) && nchar(legal_chars)>0)
    # If repl_chars is not NULL, ensure it is a character type, otherwise
    # treat it as an empty character.
    if(!is.null(repl_chars)) stopifnot(is.character(repl_chars)) else repl_chars <- ""

    if(!is.null(repl_chars))
    {
        # Replace specified illegal characters in given string.
        if(!is.null(illegal_chars)) string <- gsub(illegal_chars, repl_chars, string)

        # Remove the illegal characters defined by given legal characters.
        if(!is.null(legal_chars))
        {
            # Define illegal characters.
            ill_chars <- paste0("[^",legal_chars,"]+")
            # Replace illegal characters in given string.
            string <- gsub(ill_chars, repl_chars, string)
        }

        # Remove redundant spaces in converted string.
        if(compress_space)
        {
            # Shrink multiple consecutive spaces to one space.
            string <- gsub(" +", " ", string)
            # Remove leading and trailing spaces.
            string <- gsub("^ +| +$", "", string)
        }
    }

    # Return the converted character string.
    return(string)
}
