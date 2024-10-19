# Git Commands

## Add all changes

```
git add .
```

## Commit changes

```
git commit -m 'mention the changes made in short'
```

## Push to origin from main

```
git push origin main
```

## Push to origin sprint1 from sprint1

```
# Change the sprint1 to your branch name
git push origin sprint1
```

## Pull changes from upstream

```
# Change the `sprint` to your branch name
git pull upstream sprint1
```

## Add remote origin/upstream

```
# Add remote origin
git remote add origin http://github.com/Arup3201/Enegma

# Add remote upstream
git remote add upstream http://github.com/EnegmaIn/Enegma
```

## Track another branch of the upstream

If `foo` is the branch on the upstream that you wanna track for pushing and pulling new changes then you can do that by following,

```
git branch -set--upstream-to upstream/foo
```

## Create a new branch

```
git checkout -b branch_name
```

## Change branch of your local git repo

```
git checkout branch_name
```

## Make new branch by copying the changes of one branch

```
git checkout -b new_branch old_branch
```

## Undo the new changes

```
# Change master to your own branch name
git reset --hard master@{"1 hour ago"}
```
