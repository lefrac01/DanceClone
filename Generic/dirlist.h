//TODO: dirlist() fails with buffer overflow if filename > 200 (including path, some mp3 names are long)

class direntry
{
public:
  char filename[MAXPATHLEN];
  bool folder;
  bool statfailed;
  int extrainfoint;
  direntry(char* filename2);
};

direntry::direntry(char* filename2)
{
  sprintf(filename,"%s",filename2);
  folder = 0;
  extrainfoint = 0;
}

const int maxdirentries = 500;
direntry *direntries[maxdirentries];
int n_direntries = 0;

void deletedirentry(int number)
{
  delete direntries[number];
  direntries[number] = NULL;
  if (number < n_direntries-1)
  {
    for(int a = number; a < n_direntries; a++)
    {
      direntries[a] = direntries[a+1];
    }
  }
  direntries[n_direntries-1] = NULL;
  n_direntries = n_direntries-1;
}

#ifdef WII
void dirlist(char* directory)
{
  while(n_direntries) deletedirentry(0);
  
  char filepath[MAXPATHLEN];
  struct stat st;
  struct dirent* dirEntry = NULL;
  DIR* dirStruct;

  if ((dirStruct = opendir(directory)))
  {
    if(dirStruct->dirData != NULL)
    {
      while(  (dirEntry = readdir(dirStruct)) != NULL ) 
      {
        direntries[n_direntries] = new direntry(dirEntry->d_name);
        
        if (strcmp (directory, "/") == 0)
        {
          sprintf (filepath, "%s%s", directory, dirEntry->d_name);
        }
        else
        {
          sprintf (filepath, "%s/%s", directory, dirEntry->d_name);
        }
        if(stat(filepath, &st) == 0) 
        {
          direntries[n_direntries]->statfailed = 0;
          if(st.st_mode & S_IFDIR)
          {
            direntries[n_direntries]->folder = 1;
          }
        }
        else
        {
          direntries[n_direntries]->statfailed = 1;
        }
        n_direntries++;
      }
    }
  }
}
#endif


#ifdef WIN
void dirlist(char* directory)
{
  while(n_direntries) deletedirentry(0);
  
  char filename[MAX_PATH];
  WIN32_FIND_DATAA findFileData;
  HANDLE hFind = FindFirstFileA(directory, &findFileData);
  if(hFind != INVALID_HANDLE_VALUE)
  {
    bool endoflist = 0;
    while(endoflist == 0)
    {
      sprintf(filename, "%s", findFileData.cFileName);

      direntries[n_direntries] = new direntry((char*)filename);
      n_direntries++;
      
      if(findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      {
        direntries[n_direntries]->folder = 1;
      }
      
      if(FindNextFileA(hFind, &findFileData) == 0)
      {
        endoflist = 1;
      }
    }
    //CloseHandle(hFind); //not compiled under win yet so leaving commented...
  }
}
#endif
