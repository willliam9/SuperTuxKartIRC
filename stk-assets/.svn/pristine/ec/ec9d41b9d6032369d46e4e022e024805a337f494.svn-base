#!/usr/bin/php
<?php
/* debian/copyright file checker SVN hook
 * Copyright 2017 Matthew Trescott
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
class ParserException extends Exception
{
    public function __construct($message = "", $code = 0, $previous = null)
    {
        $message = "The copyright file parser could not parse the file: " . $message . "\n\nPlease visit https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/.";
        parent::__construct($message, $code, $previous);
    }
}
class DebianCopyrightFile
{
    /**
     * This attribute contains the parsed data. Its attributes have the same names of the fields
     * in the header paragraph, but lowercase and with underscores instead of hyphens (except for
     * 'format_url' instead of just 'format'. It also has three other attributes:
     * 
     *   - standalone_licenses: array of arrays containing the data from the License and Comment
     *                          fields of a standalone license paragraph, in the 'license' and
     *                          'comment' keys, respectively.
     * 
     *   - file_descriptors: array with keys being the file descriptors from Files Paragraphs
     *                       and values being the index of the corresponding copyright and license
     *                       information in the file_paragraph_infos array.
     * 
     *   - file_paragraph_infos: array with keys being paragraph indices and values being arrays
     *                           containing the Copyright, License, and Comment fields extracted
     *                           from that paragraph, stored in the 'copyright', 'license', and
     *                           'comment' keys, respectively.
     */
    protected $data;
    
    protected $paragraph_linenumbers;
    protected $source;
    protected $parsed = false;
    protected $file_prefix;
    protected $file_path;
    
    const TKN_ASTERISK = 1;
    const TKN_QMARK = 2;
    
    /** Constructor
     * 
     * @param string $source The contents of a debian/copyright file
     * @param string $file_path The path to the file, used in error messages
     * @param string $file_prefix = "" A prefix added to Files: entries when using DebianCopyrightFile::get_file_info_abs()
     * @param string $support_crlf = false Whether to convert CRLF (Windows line ending) files to LF before parsing.
     */
    public function __construct($source, $file_path, $file_prefix = "", $support_crlf = false)
    {
        $this->source = $source;
        $this->file_prefix = $file_prefix;
        $this->data = new stdClass();
        $this->file_path = $file_path;
        
        if ($support_crlf)
        {
            $this->source = str_replace("\r", "", $this->source);
        }
        
        if (!$this->source)
        {
            throw new ParserException("Empty source provided to parser for file {$this->file_path}.");
        }
    }
    
    public function is_parsed()
    {
        return $this->parsed;
    }
    
    public function parse()
    {
        $paragraphs = array();
        $this->paragraph_linenumbers = array();
        
        $paragraph_start = 0;
        $last_lf_pos = -1;
        for ($i = 0; $i < strlen($this->source); $i++)
        {
            if ($this->source[$i] == "\n")
            {
                $line = substr($this->source, $last_lf_pos + 1, $i - $last_lf_pos - 1);
                
                if (preg_match('~[^\t ]~', $line) === 0)
                {
                    array_push($paragraphs, substr($this->source, $paragraph_start, $i - $paragraph_start));
                    array_push($this->paragraph_linenumbers, substr_count($this->source, "\n", 0, $paragraph_start) + 1);
                    $paragraph_start = $i + 1;
                }
                $last_lf_pos = $i;
            }
        }
        // Since the paragraph separator above only adds a paragraph when it reaches a "blank" line, it is
        // necessary to add the last paragraph outside of the loop.
        array_push($paragraphs, substr($this->source, $paragraph_start));
        array_push($this->paragraph_linenumbers, substr_count($this->source, "\n", 0, $paragraph_start) + 1);
        
        // Remove paragraphs that are empty or only contain spaces/tabs. This occurs when
        // multiple blank lines are used to separate paragraphs.
        // Also clean leading/trailing newlines and spaces and stuff.
        foreach ($paragraphs as $key => $paragraph)
        {
            if (preg_match('~[^\t ]~', $paragraph) === 0)
            {
                unset($paragraphs[$key]);
                unset($this->paragraph_linenumbers[$key]);
            }
            else
            {
                $paragraphs[$key] = ltrim(rtrim($paragraph));
            }
        }
        
        // Order the paragraphs nicely, starting from 0, with no skipped numbers
        $paragraphs = array_values($paragraphs);
        $this->paragraph_linenumbers = array_values($this->paragraph_linenumbers);
        
        // Parse the header paragraph
        $header_raw = $this->parse_paragraph($paragraphs[0], 0);
        $header_info = $header_raw['data'];
        $h_linenumbers = $header_raw['linenumbers'];
        unset($paragraphs[0]);
        
        if (!isset($header_info['format']))
        {
            throw new ParserException("The file {$this->file_path} appears to be lacking a header paragraph. This is the first\nparagraph of the file, and should contain at least a Format field.");
        }
        if (count($paragraphs) === 0)
        {
            throw new ParserException("The file {$this->file_path} must contain at least one Files paragraph.");
        }
        
        $this->data->format_url         = isset($header_info['format'])             ? $this->trim_single_line_field($header_info['format'],          0, $h_linenumbers['format'])           : null;
        $this->data->upstream_name      = isset($header_info['upstream-name'])      ? $this->trim_single_line_field($header_info['upstream-name'],   0, $h_linenumbers['upstream-name'])    : null;
        $this->data->upstream_contact   = isset($header_info['upstream-contact'])   ? $this->parse_line_based_list($header_info['upstream-contact'], 0, $h_linenumbers['upstream-contact']) : null;
        $this->data->source             = isset($header_info['source'])             ? $this->trim_multiline_field($header_info['source'],            0, $h_linenumbers['source'])           : null;
        $this->data->disclaimer         = isset($header_info['disclaimer'])         ? $this->trim_multiline_field($header_info['disclaimer'],        0, $h_linenumbers['disclaimer'])       : null;
        $this->data->comment            = isset($header_info['comment'])            ? $this->trim_multiline_field($header_info['comment'],           0, $h_linenumbers['comment'])          : null;
        $this->data->license            = isset($header_info['license'])            ? $this->trim_multiline_field($header_info['license'],           0, $h_linenumbers['license'])          : null;
        $this->data->copyright          = isset($header_info['copyright'])          ? $this->trim_multiline_field($header_info['copyright'],         0, $h_linenumbers['copyright'])        : null;
        
        
        $this->data->standalone_licenses = array();
        $this->data->file_descriptors = array();
        $this->data->file_paragraph_infos = array();
        
        // Parse all other paragraphs (files paragraphs and standalone license paragraphs)
        foreach ($paragraphs as $paragraph_index => $paragraph_text)
        {
            $p_raw = $this->parse_paragraph($paragraph_text, $paragraph_index);
            $p_info = $p_raw['data'];
            $field_linenumbers = $p_raw['linenumbers'];
            /* Files Paragraphs are required to have the Files, Copyright, and License fields. Paragraphs lacking
             * any of the fields required for the Files Paragraph are considered Standalone License Paragraphs,
             * provided they contain a License field. Any paragraph not containing at least a License field
             * shall be considered an error.
             */
            if (!isset($p_info['license']))
            {
                throw new ParserException("The paragraph beginning at line {$this->paragraph_linenumbers[$paragraph_index]}\nin file {$this->file_path} is missing a License field.\nThis is a required field in all debian/copyright file paragraphs.");
            }
            elseif (!isset($p_info['files']) || !isset($p_info['copyright']))
            {
                // This is a Standalone License Paragraph
                array_push(
                    $this->data->standalone_licenses,
                    array(
                        'license' => $this->trim_multiline_field($p_info['license'], $paragraph_index, $field_linenumbers['comment']),
                        'comment' => isset($p_info['comment']) ? $this->trim_multiline_field($p_info['comment'], $paragraph_index, $field_linenumbers['comment']) : null
                    )
                );
            }
            elseif (isset($p_info['files']) && isset($p_info['copyright']))
            {
                // This is a Files Paragraph
                // Store the license and copyright info for these files
                $this->data->file_paragraph_infos[$paragraph_index] = array(
                    'copyright' => $this->trim_multiline_field($p_info['copyright'], $paragraph_index, $field_linenumbers['copyright']),
                    'license' => $this->trim_multiline_field($p_info['license'], $paragraph_index, $field_linenumbers['license']),
                    'comment' => isset($p_info['comment']) ? $this->trim_multiline_field($p_info['comment'], $paragraph_index, $field_linenumbers['comment']) : null
                );
                $files = self::parse_whitespace_separated_list($p_info['files']);
                foreach ($files as $file)
                {
                    $this->data->file_descriptors[$file] = $paragraph_index;
                }
            }
            else // I don't think this EVER gets executed!
            {
                throw new ParserException("The paragraph beginning at line {$this->paragraph_linenumbers[$paragraph_index]}\nin {$this->file_path} is missing the fields necessary to qualify as either a Files or Standalone License Paragraph.");
            }
        }
        
        $this->parsed = true;
    }
    
    protected function parse_paragraph($paragraph_text, $paragraph_index)
    {
        $field_data_offsets = array();
        $field_linenumbers = array();
        $is_new_line = true;
        $current_line = $this->paragraph_linenumbers[$paragraph_index];
        $previous_field_name = null;
        for ($i = 0; $i < strlen($paragraph_text); ++$i)
        {
            if ($is_new_line)
            {
                $is_new_line = false;
                if (preg_match('~[!"$-,.-9;-\~]~', $paragraph_text[$i])) // Field names cannot start with hyphens or hashtags; or contain spaces, control codes, or colons
                {
                    // This should be a field name on this line. Get the name of the field, and figure out where its data starts.
                    $matches = array();
                    if (!preg_match('~\G([!-9;-\~]+)(:)~', $paragraph_text, $matches, PREG_OFFSET_CAPTURE, $i))
                    {
                        throw new ParserException("On line {$current_line} in file {$this->file_path}:\nInvalid character in field name or misformatted continuation line.\nSee https://www.debian.org/doc/debian-policy/ch-controlfields#s-controlsyntax");
                    }
                    
                    $field_name = $matches[1][0];
                    $field_data_begin = $matches[2][1] + 1;
                    $field_data_offsets[$field_name] = array('start' => $field_data_begin);
                    $field_linenumbers[strtolower($field_name)] = $current_line;
                    
                    if ($previous_field_name !== null)
                    {
                        // Note that the "end" offset is probably an LF character and shouldn't be included in the actual data. The minus 1 is because we are already on to the next field.
                        $field_data_offsets[$previous_field_name]['end'] = $i - 1;
                    }
                    
                    $previous_field_name = $field_name;
                }
                elseif ($paragraph_text[$i] != "\t" && $paragraph_text[$i] != " ")
                {
                    throw new ParserException("On line {$current_line} in file {$this->file_path}:\nInvalid character in field name or misformatted continuation line.\nSee https://www.debian.org/doc/debian-policy/ch-controlfields#s-controlsyntax");
                }
            }
            if ($paragraph_text[$i] == "\n")
            {
                $is_new_line = true;
                ++$current_line;
            }
            if ($i == strlen($paragraph_text) - 1)
            {
                $field_data_offsets[$previous_field_name]['end'] = $i + 1;
            }
        }
        
        $field_data = array();
        foreach ($field_data_offsets as $field => $offsets)
        {
            // Field names are case-insensitive
            $field_data[strtolower($field)] = substr($paragraph_text, $offsets['start'], $offsets['end'] - $offsets['start']);
        }
        
        return array('data' => $field_data, 'linenumbers' => $field_linenumbers);
    }
    
    protected function trim_single_line_field($str, $paragraph_index, $field_linenumber)
    {
        if (strpos($str, "\n") !== false)
        {
            throw new ParserException("In file {$this->file_path} near line {$field_linenumber}:\nA single-line field may not span multiple lines.");
        }
        
        // Trim whitespace from ends
        $matches = array();
        preg_match('~([^\t ].*?)[ \t]*$~', $str, $matches);
        
        if (!$matches)
        {
            throw new ParserException("In file {$this->file_path} near line {$field_linenumber}:\nA field should contain at least one non-whitespace character, and line-based lists should not have empty lines.");
            return null;
        }
        
        return $matches[1];
    }
    
    protected function trim_multiline_field($str, $paragraph_index, $field_linenumber)
    {
        // Just trim leading and trailing whitespace
        $matches = array();
        preg_match('~([^\t\n ].*?)[\t\n ]*$~s', $str, $matches);
        
        if (!$matches)
        {
            throw new ParserException("In file {$this->file_path} on/near line {$field_linenumber}:\nA field should contain at least one non-whitespace character.");
        }
        
        return $matches[1];
    }
    
    protected static function parse_whitespace_separated_list($str)
    {
        return preg_split('~[\n\t ]~', $str, null, PREG_SPLIT_NO_EMPTY);
    }
    
    protected  function parse_line_based_list($str, $paragraph_index, $field_linenumber)
    {
        $str = $this->trim_multiline_field($str, $paragraph_index, $field_linenumber);
        
        $raw_entries = explode("\n", $str);
        $entries = array();
        foreach ($raw_entries as $index => $entry)
        {
            if ($cleaned_field = $this->trim_single_line_field($entry, $paragraph_index, $field_linenumber + $index))
            {
                array_push($entries, $cleaned_field);
            }
        }
        
        return $entries;
    }
    
    public function comp_file_descriptors($pattern, $test)
    {
        $pattern = ltrim($pattern, '/');
        
        // First we will build an array of tokens from the pattern
        $tokens = array();
        for ($i = 0; $i < strlen($pattern); ++$i)
        {
            switch ($pattern[$i])
            {
                case "\\":
                    if ($i < strlen($pattern) - 1) // $i is not the last offset in the pattern string
                    {
                        switch ($pattern[$i + 1])
                        {
                            case "\\":
                                break;
                            case "*":
                                break;
                            case "?":
                                break;
                            default:
                                throw new ParserException("In file {$this->file_path} near token {$pattern}:\nA backslash can only precede an asterisk, a question mark, or another backslash in a file pattern.");
                        }
                        array_push($tokens, $pattern[$i + 1]);
                        ++$i; // Since an escape sequence is treated like a single character, move the iterator ahead by one.
                    }
                    else
                    {
                        throw new ParserException("In file {$this->file_path} near token {$pattern}:\nAn unescaped backslash cannot end a file pattern.");
                    }
                    break;
                case "*":
                    array_push($tokens, self::TKN_ASTERISK);
                    break;
                case "?":
                    array_push($tokens, self::TKN_QMARK);
                    break;
                default:
                    array_push($tokens, $pattern[$i]);
            }
        }
        
        // Now we will try to compare the two strings
        $text_offset = 0;
        $asterisk_in_effect = false;
        $raise_text_offset = false;
        $txt_backtrack_pt = null;
        $previously_matched = false;
        for ($tokens_offset = 0; $tokens_offset < count($tokens); ++$tokens_offset)
        {
            /* If we've reached the end of the test string, but not the end of the pattern, and the pattern still has
             * characters that need to match (i.e. not an asterisk), the strings do not match.
             */
            if ($text_offset === strlen($test) - 1 && $tokens_offset < count($tokens) - 1 && max(array_keys(preg_grep('~[^*]~', $tokens))) > $tokens_offset)
            {
                return false;
            }
            
            if ($tokens[$tokens_offset] === self::TKN_ASTERISK)
            {
                /* For asterisks, we will keep trying to match the characters that come after it, one at a time.
                 * If, at any point, a character doesn't match, we'll backtrack in the pattern, but still advance
                 * the offset in the text by one character, and try again. Reaching the end of the test string
                 * purely by backtracking in the pattern indicates that they don't match, unless the last character
                 * in the pattern is an asterisk.
                 */
                // NOTE that asterisks are implemented here as a lazy quantifier.
                $asterisk_in_effect = true;
                $tkn_backtrack_pt = $tokens_offset;
            }
            elseif ($tokens[$tokens_offset] === self::TKN_QMARK || $tokens[$tokens_offset] === $test[$text_offset])
            {
                if (!$previously_matched && $asterisk_in_effect)
                {
                    $txt_backtrack_pt = $text_offset; // This is the last place where the pattern following the asterisk began to match
                    $previously_matched = true;
                }
                $raise_text_offset = true;
            }
            else
            {
                if ($asterisk_in_effect === true) // Backtrack
                {
                    if ($txt_backtrack_pt !== null)
                    {
                        $text_offset = $txt_backtrack_pt + 1;
                        $txt_backtrack_pt = null;
                    }
                    /* It does not make sense to advance the text offset when there is a match failure,
                     * because there is a chance that by backtracking, that same text offset might be
                     * matched at the backtrack point.
                     * 
                     * However, this can cause an infinite loop when backtracking only goes back by
                     * one character and simply returns the algorithm to the same character in the
                     * pattern that it just tested. This is because if the first character after the
                     * backtrack point doesn't match, there is no sense in trying to backtrack, since
                     * the same problem will just come up again. Instead, just move the text offset ahead.
                     */
                    if ($tokens_offset === $tkn_backtrack_pt + 1)
                    {
                        $raise_text_offset = true;
                    }
                    $tokens_offset = $tkn_backtrack_pt;
                    $previously_matched = false;
                }
                else
                {
                    return false;
                }
            }
            /* If we have reached the end of the pattern, but not the end of the test string, and there was an asterisk,
             * there is the possibility that there is a correct match farther along.
             */
            if ($tokens_offset === count($tokens) - 1 && $text_offset < strlen($test) - 1 && $asterisk_in_effect)
            {
                $tokens_offset = $tkn_backtrack_pt;
                $text_offset = $txt_backtrack_pt ? $txt_backtrack_pt + 1 : $text_offset;
            }
            
            /* If we have reached the end of the pattern, and the last character of
             * the pattern is an asterisk, the strings match.
             */
            if ($tokens_offset === count($tokens) - 1 && $text_offset < strlen($test) && $tokens[$tokens_offset] === self::TKN_ASTERISK)
            {
                return true;
            }
            elseif ($tokens_offset === count($tokens) - 1 && $text_offset < strlen($test) - 1 && $tokens[$tokens_offset] !== self::TKN_ASTERISK)
            {
                return false;
            }
            
            // this allows us to make token offsets continue to to correspond to text offsets
            // until the next iteration.
            if ($raise_text_offset === true)
            {
                ++$text_offset;
                $raise_text_offset = false;
            }
        }
        
        return true;
    }
    
    public function get_file_info_abs($abs_path)
    {
        $matches = array();
        foreach ($this->data->file_descriptors as $file_descriptor => $info_index)
        {
            if ($this->comp_file_descriptors($this->file_prefix . $file_descriptor, $abs_path))
            {
                array_push($matches, $info_index);
            }
        }
        if ($matches)
        {
            return $this->data->file_paragraph_infos[array_pop($matches)]; // The last matching file descriptor is used per the debian/copyright file format spec.
        }
        else
        {
            return false;
        }
    }
    
    public function get_format_url()
    {
        return $this->data->format_url;
    }
    
    public function get_upstream_name()
    {
        return $this->data->upstream_name;
    }
    
    public function get_upstream_contacts()
    {
        return $this->data->upstream_contact;
    }
    
    public function get_upstream_url()
    {
        return $this->data->source;
    }
    
    public function get_overall_license()
    {
        return $this->data->license;
    }
}
    
class SvnWrapper
{
    const REV_TYPE_TXN = 0;
    const REV_TYPE_REV = 1;
    
    protected $svnlook_path;
    protected $repo_path;
    
    public function __construct($svnlook_path, $repo_path)
    {
        $this->repo_path = $repo_path;
        $this->svnlook_path = $svnlook_path;
    }
    
    protected function parse_changes_list($svnlook_output)
    {
        $changes = explode("\n", $svnlook_output);
        if (!$changes)
        {
            return array();
        }
        array_pop($changes); // Remove last element, which is an empty string due to a trailing linefeed
        
        $changes_parsed = array();
        foreach ($changes as $raw_listing)
        {
            $matches = array();
            if (preg_match('~^(.+?)[ ]+(.+)$~', $raw_listing, $matches))
            {
                // Trim the svn repo path from beginning
                $matches[2] = ltrim(substr($matches[2], strlen($this->repo_path)), '/');
                $changes_parsed[$matches[2]] = $matches[1] == 'M' ? 'U' : $matches[1]; // The key is the file path, the value is the type of change made.
            }
            // We no longer bother checking for non-matches because unlike svnlook, svn status shows moves, not just additions and deletions
        }
        return $changes_parsed;
    }
    
    public function get_changes_by_type()
    {
        $cmd = "{$this->svnlook_path} status --quiet {$this->repo_path}";
        
        $list = $this->parse_changes_list(shell_exec($cmd));
        $changes_by_type = array(
            'A' => array(),
            'U' => array(),
            'D' => array(),
            '_U' => array(),
            'UU' => array()
        );
        foreach ($list as $file => $change_type)
        {
            array_push($changes_by_type[$change_type], $file);
        }
        return $changes_by_type;
    }
    
    public function cat_file($path)
    {
        $path = '/' . ltrim($path, '/');
        
        return (file_exists($this->repo_path . $path) ? file_get_contents($this->repo_path . $path) : false);
    }
}
/** Puts an ASCIIart outline around the text for display in a console.
 * @param string $str The input string.
 * @return string The formatted string.
 */
function outline_string($str)
{
    $lines = explode("\n", $str);
    $lengths = array();
    foreach ($lines as $line)
    {
        array_push($lengths, strlen($line));
    }
    $max_len = max($lengths);
    $num_hyphens = $max_len + 2;
    $out = '+' . str_repeat('-', $num_hyphens) . "+\n";
    foreach ($lines as $line)
    {
        $out .= "| $line" . str_repeat(' ', $max_len - strlen($line)) . " |\n";
    }
    $out .= '+' . str_repeat('-', $num_hyphens) . "+\n";
    return $out;
}
/** Pads a string with spaces to center it for a given width
 * @param string $str The string to center
 * @param int $width The number of characters in the line
 */
function center_string($str, $width)
{
    $num_spaces = floor(($width - strlen($str)) / 2);
    return str_repeat(' ', $num_spaces) . $str;
}
/** Flush output, appending a thank-you message.
 * @param int $status The status code to return, nonzero for failure.
 */
function finish_up($status)
{
    global $organization_name;
    echo "\nThank you for helping to keep {$organization_name} copyright-compliant. Have a nice day!\n";
    $out_parts = explode("\n", ob_get_clean());
    $lines_length = array();
    foreach ($out_parts as $line)
    {
        array_push($lines_length, strlen($line));
    }
    $out_parts[0] = center_string($out_parts[0], max($lines_length));
    $out_parts[1] = center_string($out_parts[1], max($lines_length));
    $out = outline_string(implode("\n", $out_parts));
    
    fwrite(STDOUT, $out);
}
    
function execute()
{
    global $svnlook_path, $argv, $include_dirs, $local_priority;
    
    if (!preg_match('~.*stk-assets~', getcwd(), $path_result))
    {
        throw new Exception("Current directory is not in stk-assets.");
    }
    /* If we need to stop the commit, we'll need to write to STDERR instead of STDOUT. If we don't
     * need to cancel the commit, but still want to relay a message to the client, we'll write to
     * a temporary file, which a post-commit hook will write to STDERR before returning a nonzero
     * exit code in order for SVN to relay the message.
     */
    ob_start();
    
    $svn_repo_path = $path_result[0];
    $svnlook = new SvnWrapper('/usr/bin/svn', $svn_repo_path);
    $changes = $svnlook->get_changes_by_type();
    
    try
    {
        $copyright_files = array(); // Keys are paths to license files, values are instances of DebianCopyrightFile
        
        $global_file_jurisdictions = array(); // Keys are paths to license files, values are "jurisdictions" (paths to the parent directory of the license file)
        foreach($include_dirs as $jurisdiction => $license_filename) // Make an array of copyright files from $include_dirs
        {
            // First we need to clean up the $include_dirs
            if ($license_filename === false)
            {
                continue; // This is an include dir that exclusively uses local license files
            }
            
            $trimmed_dir = rtrim(ltrim($jurisdiction, '/'), '/');
            $trimmed_license_path = ltrim("$trimmed_dir/" . ltrim($license_filename, '/'), '/'); // We need to trim leading slashes if $trimmed_dir === ''.
            
            if ($svnlook->cat_file($trimmed_license_path) === false)
            {
                fwrite(STDERR, "Invalid license path provided in \$include_dirs configuration! Contact SVN repo admin!\n");
                exit(2);
            }
            
            $global_file_jurisdictions[$trimmed_license_path] = $trimmed_dir;
            
            $copyright_files[$trimmed_license_path] = new DebianCopyrightFile(
                $svnlook->cat_file($trimmed_license_path),
                $trimmed_license_path,
                $trimmed_dir ? $trimmed_dir . '/' : '', // Jurisdiction for the root folder is an empty string; we don't want leading slashes when comparing globs.
                true
            );
            $copyright_files[$trimmed_license_path]->parse();
        }
        
        $local_file_jurisdictions = array(); // Keys are paths to license files, values are "jurisdictions" (paths to the parent directory of the license file)
        foreach (array_merge($changes['A'], $changes['U'], $changes['UU']) as $changed_file) // Find and add copyright files contained in the same directories as the files being changed on-the-fly
        {
            if ($changed_file[strlen($changed_file) - 1] === '/')
            {
                continue; // Skip directories
            }
            
            if (preg_match('~(.+)\\/.+~', $changed_file, $matches))
            {
                $local_jurisdiction = $matches[1]; // The path to the directory containing the $changed_file, without a trailing slash
            }
            else
            {
                $local_jurisdiction = '';
            }
            if (!isset($local_copyright_files[$local_jurisdiction]) && ($local_license_file = $svnlook->cat_file("{$local_jurisdiction}/licenses.txt")) !== false)
            {
                $local_license_file_path = ltrim("$local_jurisdiction/licenses.txt", '/'); // We need to trim leading slashes if $local_jurisdiction === ''.
                
                $local_file_jurisdictions[$local_license_file_path] = $local_jurisdiction;
                
                $copyright_files[$local_license_file_path] = new DebianCopyrightFile(
                    $local_license_file,
                    $local_license_file_path,
                    $local_jurisdiction ? "{$local_jurisdiction}/" : '', // Jurisdiction for the root folder is an empty string; we don't want leading slashes when comparing globs.
                    true
                );
                $copyright_files[$local_license_file_path]->parse();
            }
        }
    }
    
    catch (ParserException $e)
    {
        echo "The Debian Copyright file parser encountered an error.\n";
        echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        echo $e->getMessage();
        echo "\n";
        finish_up(3);
    }
    
    $changed_files = array(); // Keys are changed files, values are arrays of paths to applicable license files
    foreach (array_merge($changes['A'], $changes['U'], $changes['UU']) as $changed_file) // Go through each changed file and decide which license file(s) apply to it.
    {
        // We first need to check whether this file should actually be checked for documentation.
        if ($changed_file[strlen($changed_file) - 1] == "/" || is_dir($svn_repo_path . '/' . $changed_file))
        {
            continue; // If the last character of the file path is a slash, skip it; it's a directory.
        }
        
        $matched_include_dir = false;
        foreach ($include_dirs as $jurisdiction => $license_filename)
        {
            $trimmed_jurisdiction = rtrim(ltrim($jurisdiction, '/'), '/');
            if (!$trimmed_jurisdiction || strpos($changed_file, $trimmed_jurisdiction) === 0)
            {
                $matched_include_dir = true;
            }
        }
        if (!$matched_include_dir)
        {
            continue;
        }
        
        /* The next thing we need to do is to figure out which copyright file applies to
         * this file. For example, a file in stk-assets should use the file
         * stk-assets/licenses.txt because stk-assets/licenses.txt has the jurisdiction of stk-assets/.
         * However, this could also be another licenses.txt discovered on-the-fly (a "local" license file)
         */
        $applicable_local_license_file = null;
        $applicable_global_license_file = null;
        foreach ($global_file_jurisdictions as $license_file_path => $jurisdiction)
        {
            if (!$jurisdiction || strpos($changed_file, $jurisdiction) === 0) // If "jurisdiction" is empty, then it applies everywhere.
            {
                $applicable_global_license_file = $license_file_path;
            }
        }
        foreach ($local_file_jurisdictions as $license_file_path => $jurisdiction)
        {
            /* For local license files, it is not sufficient that we check merely that the jurisdiction of each local license file
             * starts the path to this file. Local license files MUST NOT be applied to files at any other directory level than
             * their own.
             */
            if (!$jurisdiction) // Empty string, i.e. applies to root directory
            {
                if (strpos($changed_file, '/') === false) // Changed file should not include a slash if it is in the root directory
                {
                    $applicable_local_license_file = $license_file_path;
                }
            }
            elseif (strpos($changed_file, $jurisdiction) === 0)
            {
                // Changed file should not contain a slash after the the jurisdiction and the slash separating the jurisdiction and the file itself.
                if (strpos($changed_file, '/', strlen($jurisdiction) + 1) === false)
                {
                    $applicable_local_license_file = $license_file_path;
                }
            }
        }
        
        if ($changed_file === $applicable_local_license_file || $changed_file === $applicable_global_license_file)
        {
            continue; // If the modified file is the copyright file itself, don't check it.
        }
        
        $changed_files[$changed_file] = array(
            'local' => $applicable_local_license_file,
            'global' => $applicable_global_license_file
        );
        
        // If the local license file is NULL because a licenses.txt file does not yet exist, but $local_priority tells us that the committer should be notified to create
        // that local file, we will add a new 'fake_local' file path. The regex is copied from the local license file discovery loop.
        if (($local_priority || !$applicable_global_license_file) && $applicable_local_license_file === null)
        {
            if (preg_match('~(.+)\\/.+~', $changed_file, $matches))
            {
                $fake_local_file = $matches[1] . '/licenses.txt';
            }
            else
            {
                $fake_local_file = 'licenses.txt'; // This file is the repository root
            }
            $changed_files[$changed_file]['fake_local'] = $fake_local_file;
        }
    }
    
    try
    {
        $undocumented_additions_by_file = array(); // For both of these arrays, keys are license file paths and values are arrays of the changed files to which the license file applies
        $documented_changes_by_file = array();
        $undocumented_modifications_by_file = array();
        foreach ($changed_files as $file_path => $license_file_paths)
        {
            /* The best license file is the path to the file in which, ideally, this
             * changed file would be documented. $local_priority, if TRUE, tells us 
             * that the best license file is a decentralized one discovered on-the-fly.
             */
            if ($local_priority)
            {
                $best_license_file = isset($license_file_paths['fake_local']) ? $license_file_paths['fake_local'] : $license_file_paths['local'];
            }
            else
            {
                if ($license_file_paths['global'] === null)
                {
                    if ($license_file_paths['local'] === null)
                    {
                        $best_license_file = $license_file_paths['fake_local'];
                    }
                    else
                    {
                        $best_license_file = $license_file_paths['local'];
                    }
                }
                else
                {
                    $best_license_file = $license_file_paths['global'];
                }
            }
            
            if (
                ($license_file_paths['local'] && $copyright_files[$license_file_paths['local']]->get_file_info_abs($file_path))
                || ($license_file_paths['global'] && $copyright_files[$license_file_paths['global']]->get_file_info_abs($file_path))
            ) {
                // The file is documented, at least somewhere.
                if (!isset($documented_changes_by_file[$best_license_file]) && !in_array($file_path, $changes['A'])) // Don't remind committers to update the license file for newly-added files.
                {
                    $documented_changes_by_file[$best_license_file] = array();
                }
                if (!in_array($file_path, $changes['A']))
                {
                    array_push($documented_changes_by_file[$best_license_file], $file_path);
                }
            }
            else
            {
                // The file needs to be documented.
                if (in_array($file_path, $changes['A']))
                {
                    // Undocumented additions should block the commit
                    if (!isset($undocumented_additions_by_file[$best_license_file]))
                    {
                        $undocumented_additions_by_file[$best_license_file] = array();
                    }
                    array_push($undocumented_additions_by_file[$best_license_file], $file_path);
                }
                elseif (in_array($file_path, $changes['U']) || in_array($file_path, $changes['UU']))
                {
                    if (!isset($undocumented_modifications_by_file[$best_license_file]))
                    {
                        $undocumented_modifications_by_file[$best_license_file] = array();
                    
                    }
                    array_push($undocumented_modifications_by_file[$best_license_file], $file_path);
                }
            }
        }
    }
    
    catch (ParserException $e) // Catch badly-formatted file glob syntax
    {
        echo "The Debian Copyright file parser encountered an error.\n";
        echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        echo $e->getMessage();
        echo "\n";
        finish_up(4);
    }
    
    echo "The Debian Copyright file checker has finished checking your files.\n";
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
    if (!$undocumented_additions_by_file)
    {
        echo "No changes require license updates. You may ignore any further errors.\n";
    }
    
    // Undocumented Files
    if ($undocumented_additions_by_file)
    {
        foreach ($undocumented_additions_by_file as $license_file_path => $files)
        {
            echo "The following files you have attempted to add are missing from the license database file \"{$license_file_path}\":\n";
            foreach ($files as $file)
            {
                echo "     {$file}\n";
            }
            echo "\n";
        }
        
        echo "-----------\n";
        echo "Your commit will therefore be blocked. I apologize for this inconvenience.\n";
        echo "It is, however, imperative that our assets remain in compliance with copyright law.\n";
        echo "Please see https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/\n";
        echo "for instructions on how to format the licenses database file.\n";
    }
    
    foreach ($undocumented_modifications_by_file as $license_file_path => $files)
    {
        echo "\n";
        echo "[Optional] The following files you have modified are missing license documentation\n";
        echo "           in the copyright database \"{$license_file_path}\".\n";
        echo "           Since you are probably not the original author of these, but are likely\n";
        echo "           only making small modifications, these will NOT block the commit. However,\n";
        echo "           please add license information at a time convenient to you, and don't forget\n";
        echo "           to include yourself in the copyright notice.\n";
        
        foreach ($files as $file)
        {
            echo "                {$file}\n";
        }
        echo "\n";
    }
    
    // Documented Files
    foreach ($documented_changes_by_file as $license_file_path => $files)
    {
        echo "\n";
        echo "[Optional] If you have made significant, creative modifications to any of the following files,\n";
        echo "           please don't forget to add your name to the copyright notice or update the copyright\n";
        echo "           year in the copyright database \"{$license_file_path}\":\n";
        
        foreach ($files as $file)
        {
            echo "                {$file}\n";
        }
        echo "\n";
    }
    
    // Finish up and send
    $end_code = $undocumented_additions_by_file ? 1 : 0;
    finish_up($end_code);
}
ini_set('max_execution_time', 10); // Don't tie up SourceForge
ignore_user_abort(true); // Just in case
# BEGIN user configuration
# Name of organization used in messages to committers
$organization_name = 'SuperTuxKart';
# Array of global license files.
# The format is 'DIRECTORY PATH' => 'NAME OF LICENSE FILE INSIDE DIRECTORY',
# (with quotes). The license filename can be FALSE to tell this program to
# rely exclusively on per-folder licenses.txt files.
$include_dirs = array(
    '' => 'licenses.txt'
);
# When informing committers to update/create a license file, if this is TRUE
# they will be told to use a local file, if not, they will be told to update
# the centralized license file.
$local_priority = true; 
# END user configuration
execute();
