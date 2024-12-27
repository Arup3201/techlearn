# Git Commands

## Add all changes

```sh
git add .
```

## Commit changes

```sh
git commit -m 'mention the changes made in short'
```

## Push to origin from main

```sh
git push origin main
```

## Push to origin sprint1 from sprint1

```sh
# Change the sprint1 to your branch name
git push origin sprint1
```

## Pull changes from upstream

```sh
# Change the `sprint` to your branch name
git pull upstream sprint1
```

## Add remote origin/upstream

```sh
# Add remote origin
git remote add origin http://github.com/Arup3201/Enegma

# Add remote upstream
git remote add upstream http://github.com/EnegmaIn/Enegma
```

## Track another branch of the upstream

If `foo` is the branch on the upstream that you wanna track for pushing and pulling new changes then you can do that by following,

```sh
git branch -set--upstream-to upstream/foo
```

## Create a new branch

```sh
git checkout -b branch_name
```

## Change branch of your local git repo

```sh
git checkout branch_name
```

## Make new branch by copying the changes of one branch

```sh
git checkout -b new_branch old_branch
```

## Undo the new changes

```sh
# Change master to your own branch name
git reset --hard master@{"1 hour ago"}
```

## Move changes from one branch to another branch

```sh
# In branch sprint1
git stash

# Move to evenflow
git switch evenflow

# Get the stashed changes
git stash pop
```

## Check the difference using `diff`

`git diff` lists out the changes between your current working directory and your staging area.

![alt text](https://www.freecodecamp.org/news/content/images/2022/03/1-1.png)

Line 1 - It's two versions of the same file. Git named it A for the 1st version and B for the 2nd version.

A - Old version of file
B - New version of file

![alt text](https://www.freecodecamp.org/news/content/images/2022/03/2-2.png)

Line 2 - Meta data about the file which is not super useful to your life. The first two hashes are concerned with two files that are compared. 100644 is an internal file mode identifier.

![alt text](https://www.freecodecamp.org/news/content/images/2022/03/3-1.png)

Line 3 - Git assigned a minus sign (-) to the A version of the file and a plus sign (+) to the B version of the file.

![alt text](https://www.freecodecamp.org/news/content/images/2022/03/4--.png)

Line 4 - Git normally shows a chunk of lines that has been modified, not the entire file.

The line starting with the (-) symbol came from the A version
The line starting with the (+) symbol came from the B version
Except the modified lines, which also include some lines of code before and after that chunk to show the context.

Line 5 - Every chunk starts with a Chunk header. The chunk header will be identified by @@ (at the start and end). Then, there are two sets of numbers. Can you see the -1 and +1?

- -1 means from the A version file, extracting one line starting at line 1.
- +1 means from the B version file, extracting one line starting at line 1.

If the sets look like -3,4 +3,2, then:

- -3,4 means from A version file, extracting four lines starting from line 3.
- +3,2 means from B version file, extracting two lines starting from line 3.

![alt text](https://www.freecodecamp.org/news/content/images/2022/03/endline.png)

No newline at the end of the file - The text says that there are no lines after those modified lines. This means that, in the above example, I added only one line and I modified the same line. So, there are no more lines after that.

Before you commit your changes, you can compare the staged changes with the last commit. You can do this by adding a single flag `--staged` or `--cached`.

You can run the `git diff HEAD` command to compare the both staged and unstaged changes with your last commit.

You can also run the `git diff <branch_name1> <branch_name2>` command to compare the changes from the first branch with changes from the second branch. Order does matter when you're comparing branches. So diff's outcome will change depending on the order.

You can run `git diff <commit_hash> <commit_hash>` the command to compare the changes between two commits. Like branch comparison, order does matter in comparing commits.

You can run the below commands to compare the changes for specific file:

- `git diff HEAD <file_name>`
- `git diff <file_name>`
- `git diff --staged <file_name>` or `git diff --cached <file_name>`
- `git diff <branch_name1> <branch_name2> <file_name>`
- `git diff <commit_hash> <commit_hash> <file_name>`

Refer to [this article](https://www.freecodecamp.org/news/git-diff-command/)
