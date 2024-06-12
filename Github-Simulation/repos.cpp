#include "repos.h"
#include<iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
int RepoNode::lastRepoID = 0;
FileNode::FileNode(string name, string content)
{
	filename = name;
	next = nullptr;
	FileContent = content;
}
RepoNode::RepoNode(string reponame, string ownername, bool m, FileNode* readme)
{
	forkcount = 0;
	this->ownername = ownername;
	RepoName = reponame;
	mode = m;//true for public and false for private
	this->readme = readme;
	files = FileLL();
	branches = BranchLL();
	ifstream file("RepoID.txt");
	file >> this->lastRepoID;
	left = NULL;
	right = NULL;
	this->repoID = lastRepoID;
}
RepositoryTree::RepositoryTree()
{
	root = NULL;
}
void RepositoryTree::insertRepoInBST(RepoNode*& root, RepoNode* newnode) {
	if (root == nullptr) {
		root = newnode;
	}
	else {
		if (newnode->repoID < root->repoID) {
			insertRepoInBST(root->left, newnode);
		}
		else {
			insertRepoInBST(root->right, newnode);
		}
	}
}
RepoNode* RepositoryTree::searchRepoByID(int id, RepoNode* root)
{
	if (root == nullptr)
	{
		return nullptr;
	}

	if (root->repoID == id) 
	{
		
		return root;
	}
	else if (id < root->repoID)
	{
		// Search in the left subtree
		return searchRepoByID(id, root->left);
	}
	else
	{
		
		return searchRepoByID(id, root->right);
	}
}

int RepositoryTree::convertStringToInt(const string& str)
{
	stringstream ss(str);
	int result;
	ss >> result;
	return result;
}
void RepositoryTree::WriteInRepoFile(int ID, string ownername, string reponame, bool m, string readFilename)
{
	ofstream file("Repository.csv", ios::app);
	file  <<ID << ","<< ownername << "," << reponame << "," << (m = true ? "true" : "false")<< "," << readFilename << endl;
	file.close();
}
void RepositoryTree::displayTree(RepoNode* root) {
	if (root != NULL) {
		displayTree(root->left);
		cout << root->repoID << ": " << root->RepoName << endl;
		cout << root->ownername << endl;
		cout << "Files:" << endl;
		root->files.printFileNames(); // Print file names associated with the repository
		cout << "forkcont  for " << root->repoID << " is" << root->forkcount << endl;
		displayTree(root->right);
	}
}

void RepositoryTree::readFromRpoFile(RepositoryTree& tree) {
	ifstream file("Repository.csv");
	if (!file.is_open()) {
		
		return;
	}
	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string id, owner, repo, m, readme;
		getline(ss, id, ',');
		getline(ss, owner, ',');
		getline(ss, repo, ',');
		getline(ss, m, ',');
		getline(ss, readme);

		// Convert mode string to boolean
		bool mode = (m == "true");

		// Create README node
		FileNode* readmeNode = new FileNode(readme, "");

		// Create new repository node
		RepoNode* newNode = new RepoNode(repo, owner, mode, readmeNode);
		newNode->repoID = convertStringToInt(id); // Set repository ID

		// Insert new node into the tree
		tree.insertRepoInBST(tree.root, newNode);
	}
	file.close();
}

void RepositoryTree::createRepo(string ownername)
{
	string reponame;
	cout << "Enter repo name: ";
	getline(cin, reponame);
	count = count + 1;
	cout << "Choose mode" << endl;
	cout << "Enter 1 for public " << endl;
	cout << "Enter 0 for private" << endl;
	bool m;
	cin >> m;

	cin.ignore(); // Clear the newline character

	string discription;
	cout << "Enter description: ";
	getline(cin, discription);

	// Create README file
	ofstream readmeFile(reponame + "Readme.txt");
	if (readmeFile.is_open())
	{
		readmeFile << discription; // Write description to README file
		readmeFile.close();
	}
	else
	{
		cout << "Unable to create README file: " << reponame + "Readme.txt" << endl;
		return; // Exit function if unable to create README file
	}

	FileNode* readme = new FileNode(reponame + "Readme.txt", discription);
	RepoNode* newnode = new RepoNode(reponame, ownername, m, readme);
	newnode->lastRepoID++;
	newnode->repoID = newnode->lastRepoID;
	ofstream file("RepoID.txt");
	file << newnode->lastRepoID;
	file.close();
	newnode->readme->filename = reponame + "Readme.txt";

	insertRepoInBST(root, newnode);
	WriteInRepoFile(newnode->repoID, ownername, reponame, m, reponame + "Readme.txt");
}
void RepositoryTree::display_inorder(RepoNode* root)
{
	RepoNode* current = root;
	
		if (root == nullptr)
		{
			return;
		}
		display_inorder(root->left);
		cout << "Usernme Name: " << current->ownername << endl;
		cout << "Repository Name: " << current->RepoName << endl;
		cout << "Repository ID: " << current->repoID << endl;
		cout << "Mode: " << (current->mode ? "Public" : "Private") << endl;
		cout << "Fork Count: " << current->forkcount << endl;
		cout << "Total repos are:" << count << endl;
		display_inorder(root->right);
	
}
void RepositoryTree::viewProfile(const int& id)
{
	RepoNode* current = root;
	bool found = false;
	
	// Traverse the repository tree to find repositories owned by the user
	while (current != nullptr)
	{
		if (current->repoID==id) 
		{
			found = true;
			// Display repository information
			cout << "Usernme Name: " << current->ownername << endl;
			cout << "Repository Name: " << current->RepoName << endl;
			cout << "Repository ID: " << current->repoID << endl;
			cout << "Mode: " << (current->mode ? "Public" : "Private") << endl;
			cout << "Fork Count: " << current->forkcount << endl;

			// Display README if exists
			if (current->readme != nullptr) {
				cout << "README:" << endl << current->readme->FileContent << endl;
			}

			// Display files in the root directory
			cout << "Files:" << endl;
			current->files.printFileNames();

			// Display branches
			cout << "Branches:" << endl;
			BranchNode* currentBranch = current->branches.head;
			while (currentBranch != nullptr) {
				cout << "- Branch Name: " << currentBranch->branchName << endl;
				cout << "  Owner: " << currentBranch->branchowner << endl;
				cout << "  Mode: " << (currentBranch->branchmode ? "Public" : "Private") << endl;

				// Display README if exists
				if (currentBranch->branchreadme != nullptr) {
					cout << "  README:" << endl << currentBranch->branchreadme->FileContent << endl;
				}

				// Display files in the branch
				cout << "  Files:" << endl;
				currentBranch->branchfiles.printFileNames();

				currentBranch = currentBranch->next;
			}

			cout << "----------------------------------------" << endl;
		}
		current = current->right; // Move to the next repository
	}

}

void RepositoryTree::addFileRepo(const string& loggedInUser)
{
	// Prompt the user to enter the ID of the repository
	int repoID;
	cout << "Enter the ID of the repository where you want to add a file: ";
	cin >> repoID;
	cin.ignore(); // Clear the newline character from the buffer

	// Search for the repository by ID
	RepoNode* repo = searchRepoByID(repoID, root);

	// Check if the repository exists and belongs to the current user
	if (repo != nullptr && repo->ownername == loggedInUser)
	{
		// Prompt the user to enter the filename and content of the file to be added
		string file, content, type;
		cout << "Enter the filename: ";
		getline(cin, file);
		cout << "Enter the content of the file: ";
		getline(cin, content);
		cout << "Enter the type of file (e.g., txt, cpp): ";
		cin >> type;

		// Concatenate filename and type
		string filename = file + "." + type;
		ofstream f(filename);
		f << content;
		f.close();
		// Add the file to the repository's file linked list
		repo->files.insert(filename, content);
		// Optionally, you may want to update the file in the CSV or any other file storage here
		writeInFiles(repoID, filename, repo->ownername);

		cout << "File added successfully to repository " << repo->RepoName << endl;
	}
	else
	{
		cout << "You do not have permission to add files to it." << endl;
	}

}
void RepositoryTree::deleteFileRepo(string& loggedInUser)
{
	int id;
	cout << "Enter id of repo whose files you want to delete:";
	cin >> id;
	string filename;
	cin.ignore();
	cout << "Enter filename with type: ";
	getline(cin, filename);
	RepoNode* repo = searchRepoByID(id, root);
	repo->files.remove(filename);
	removeEntryFromFileCSV(id, filename);
}
void RepositoryTree::writeInBranchFiles(int repoid, string filename, string ownername, string branchname)
{
	ifstream read("BranchFiles.csv");
	vector<string> lines;
	bool idFound = false;
	string line;

	while (getline(read, line))
	{
		stringstream ss(line);
		string id, owner, BN;
		getline(ss, id, ',');
		getline(ss, owner, ',');
		getline(ss, BN, ',');
		// If repoID matches, append filename 
		if (convertStringToInt(id) == repoid && owner == ownername && branchname == BN)
		{
			line += "," + filename;
			idFound = true;
		}
		lines.push_back(line);
	}
	read.close();
	// If repoID not found, add a new line for it
	if (!idFound)
	{
		lines.push_back(to_string(repoid) + "," + ownername + "," + branchname + "," + filename);
	}
	// Rewrite the entire file with modified lines
	ofstream file("BranchFiles.csv", ios::trunc); // Open in truncation mode to clear existing content
	for (const auto& modifiedLine : lines)
	{
		file << modifiedLine << endl;
	}
	file.close();
}
void RepositoryTree::writeInFiles(int repoID, const string& filename, string ownername)
{
	ifstream read("files.csv");
	vector<string> lines;
	bool idFound = false;
	string line;
	while (getline(read, line))
	{
		stringstream ss(line);
		string id;
		getline(ss, id, ',');
		// If repoID matches, append filename 
		if (convertStringToInt(id) == repoID)
		{
			line += "," + filename;
			idFound = true;
		}
		lines.push_back(line);
	}
	read.close();
	// If repoID not found, add a new line for it
	if (!idFound)
	{
		lines.push_back(to_string(repoID) + "," + ownername + "," + filename);
	}
	// Rewrite the entire file with modified lines
	ofstream file("files.csv", ios::trunc); // Open in truncation mode to clear existing content
	for (const auto& modifiedLine : lines)
	{
		file << modifiedLine << endl;
	}
	file.close();
}
void RepositoryTree::removeEntryFromFileCSV(int repoID, const string& filename) {
	ifstream inFile("files.csv");
	if (!inFile.is_open()) {
		cerr << "Error: Unable to open input file." << endl;
		return;
	}

	ofstream outFile("temp.csv");
	if (!outFile.is_open()) {
		cerr << "Error: Unable to create output file." << endl;
		inFile.close(); // Close the input file
		return;
	}

	string line;
	// Read each line from files.csv and update the line if needed
	while (getline(inFile, line)) {
		stringstream ss(line);
		string id, owner, fileList;
		getline(ss, id, ',');
		getline(ss, owner, ',');
		getline(ss, fileList);

		// Check if the repository ID matches
		if (convertStringToInt(id) == repoID) {
			// Create a stringstream to process the list of files
			stringstream fileListStream(fileList);
			string file;
			string updatedFileList;

			// Parse the list of files, excluding the specified file
			while (getline(fileListStream, file, ',')) {
				if (file != filename) {
					updatedFileList += file + ",";
				}
			}

			// Remove the trailing comma
			if (!updatedFileList.empty()) {
				updatedFileList.pop_back();
			}

			// Write the updated line to the output file
			outFile << id << "," << owner << "," << updatedFileList << endl;
		}
		else {
			// Write the original line to the output file
			outFile << line << endl;
		}
	}

	// Close files
	inFile.close();
	outFile.close();

	// Replace the original file with the modified file
	remove("files.csv");
	rename("temp.csv", "files.csv");
}
void RepositoryTree::LoadFromBranchFiles(RepositoryTree& tree) {
	ifstream file("BranchFiles.csv");
	if (!file.is_open()) 
	{
	
		return;
	}

	string line;
	while (getline(file, line))
	{
		stringstream ss(line);
		string repoID_str, owner, branchName, filename;

		// Extract repository ID, owner name, and branch name
		getline(ss, repoID_str, ',');
		getline(ss, owner, ',');
		getline(ss, branchName, ',');

		int repoID = convertStringToInt(repoID_str);

		// Search for the repository node by ID
		RepoNode* repo = searchRepoByID(repoID, tree.root);

		// If the repository node is found, insert files into its linked list
		if (repo != nullptr) {
			while (getline(ss, filename, ',')) {
				// Insert the file into the repository's file linked list
				string content;
				ifstream fileContent(filename);
				if (fileContent.is_open()) {
					string fileLine;
					while (getline(fileContent, fileLine)) {
						content += fileLine + "\n"; // Read file content line by line
					}
					fileContent.close();

					// Insert file into the branch's file linked list
					repo->branches.insert(owner, branchName, false, nullptr); // Assuming branch mode is false for now
					repo->branches.head->branchfiles.insert(filename, content);
				}
				
			}
		}
		
	}

	file.close();
}


void RepositoryTree::LoadFromFile(RepositoryTree& tree) {
	ifstream file("files.csv");
	if (!file.is_open()) {
		
		return;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string repoID_str, owner, filename;

		// Extract repository ID and owner name
		getline(ss, repoID_str, ',');
		getline(ss, owner, ',');

		// Convert repository ID from string to integer
		int repoID = stoi(repoID_str);

		// Search for the repository node by ID
		RepoNode* repo = searchRepoByID(repoID, tree.root);

		// If the repository node is found, insert files into its linked list
		if (repo != nullptr) {
			while (getline(ss, filename, ',')) {
				// Insert the file into the repository's file linked list
				repo->files.insert(filename, ""); // Assume content is empty for now
			}
		}
		else {
			//cout << "Repository with ID " << repoID << " not found." << endl;
		}
	}
	file.close();
}
void RepositoryTree::deleteRepository(RepoNode*& repo, RepoNode*& parent, RepoNode* r) {
	if (r == nullptr) {
		cout << "Repo ID " << repo->repoID << " not found" << endl;
		return;
	}

	if (repo->repoID < r->repoID) {
		deleteRepository(repo, r, r->left);
	}
	else if (repo->repoID > r->repoID) {
		deleteRepository(repo, r, r->right);
	}
	else {
		// Delete associated data from files
		if (r->branches.head != nullptr) {
			deleteBranchFiles(r->branches.head->branchName);
			deleteBranchFromRepo(r->branches.head->branchName, repo);
		}
		deleteRepoFromFile(r->repoID);

		if (r->left == nullptr) {
			if (parent == nullptr) {
				repo = r->right; // Update root if deleting root node
			}
			else if (parent->left == r) {
				parent->left = r->right; // Update parent's left pointer
			}
			else {
				parent->right = r->right; // Update parent's right pointer
			}
			delete r;
		}
		else if (r->right == nullptr) {
			if (parent == nullptr) {
				repo = r->left; // Update root if deleting root node
			}
			else if (parent->left == r) {
				parent->left = r->left; // Update parent's left pointer
			}
			else {
				parent->right = r->left; // Update parent's right pointer
			}
			delete r;
		}
		else {
			RepoNode* temp = findMin(r->right);
			r->repoID = temp->repoID;
			deleteRepository(repo, r, r->right);
		}
	}
}



RepoNode* RepositoryTree::findMin(RepoNode* node) {
	while (node->left != NULL) {
		node = node->left;
	}
	return node;
}
void RepositoryTree::deleteBranchFromRepo(const string& branchname, RepoNode*& repo) {
	ifstream inFile("branches.csv");
	if (!inFile.is_open()) {
		cerr << "Error: Unable to open branches.csv for reading." << endl;
		return;
	}

	vector<string> lines;
	string line;

	// Read each line from branches.csv and skip lines related to the repository to be deleted
	while (getline(inFile, line)) {
		stringstream ss(line);
		string repoID, branchInfo;
		getline(ss, repoID, '_');

		// Check if the line is related to the repository
		if (convertStringToInt(repoID) != repo->repoID || line.find(branchname) == string::npos) {
			lines.push_back(line); // Add lines not related to the repository or branch to delete
		}
	}

	inFile.close();

	// Rewrite the file with modified lines
	ofstream outFile("branches.csv", ios::trunc); // Open in truncation mode to clear existing content
	for (const auto& modifiedLine : lines) {
		outFile << modifiedLine << endl;
	}
	outFile.close();
}

void RepositoryTree::deleteBranchFiles(const string& branchname) {
	ifstream inFile("BranchFiles.csv");
	if (!inFile.is_open()) {
		cerr << "Error: Unable to open BranchFiles.csv for reading." << endl;
		return;
	}
	vector<string> lines;
	string line;

	// Read each line from BranchFiles.csv and skip lines related to the branch to be deleted
	while (getline(inFile, line)) {
		stringstream ss(line);
		string repoID, owner, branchName, filename;
		getline(ss, repoID, ',');
		getline(ss, owner, ',');
		getline(ss, branchName, ',');
		getline(ss, filename, ',');

		// Check if the line is related to the branch to be deleted
		if (branchName != branchname) {
			lines.push_back(line); // Add lines not related to the branch to delete
		}
	}

	inFile.close();

	// Rewrite the file with modified lines
	ofstream outFile("BranchFiles.csv", ios::trunc); // Open in truncation mode to clear existing content
	for (const auto& modifiedLine : lines) {
		outFile << modifiedLine << endl;
	}
	outFile.close();
}

void RepositoryTree::deleteRepoFromFile(int repoID) {
	ifstream inFile("Repository.csv");
	if (!inFile.is_open()) {
		cerr << "Error: Unable to open Repository.csv for reading." << endl;
		return;
	}

	vector<string> lines;
	string line;

	// Read each line from Repository.csv and skip lines related to the repository to be deleted
	while (getline(inFile, line)) {
		stringstream ss(line);
		string id, owner, repoName, mode, filename;
		getline(ss, id, ',');
		getline(ss, owner, ',');
		getline(ss, repoName, ',');
		getline(ss, mode, ',');
		getline(ss, filename, ',');

		// Check if the line is related to the repository to be deleted
		if (convertStringToInt(id) != repoID) {
			lines.push_back(line); // Add lines not related to the repository to delete
		}
	}

	inFile.close();

	// Rewrite the file with modified lines
	ofstream outFile("Repository.csv", ios::trunc); // Open in truncation mode to clear existing content
	for (const auto& modifiedLine : lines) {
		outFile << modifiedLine << endl;
	}
	outFile.close();
}

void RepositoryTree::makeBranch(int id, string loggedInUser)
{
	string BN;
	cout << "Enter branchname: ";
	cin.ignore();
	getline(cin, BN);

	RepoNode* repo = searchRepoByID(id, root);
	BranchNode* branch = new BranchNode(loggedInUser, BN, repo->mode, repo->readme);
	BranchNode* curr = repo->branches.head;
	while (curr != NULL)
	{
		cout << "Current branch owner: " << curr->branchowner << ", Logged-in user: " << loggedInUser << endl;
		if (curr->branchowner == loggedInUser)
		{
			cout << "You already have a branch." << endl;
			return;
			bool c = iscommit();
			if (c)
			{
				addcommit(id, branch);
			}
			return;
		}
		curr = curr->next;
	}

	repo->branches.insert(loggedInUser, BN, repo->mode, repo->readme);
	cout << "You have successfully forked this repository with ID " << id << endl;
	bool ans = iscommit();
	if (ans)
	{
		addcommit(id, branch);
	}
	writeInBranches(id, branch);
}

void RepositoryTree::addcommit(int id, BranchNode* branch)
{
	cin.ignore(); // Clear newline character from buffer
	string filename;
	cout << "Enter filename: ";
	getline(cin, filename);
	string type;
	cout << "Enter type of file: ";
	getline(cin, type);
	string content;
	cout << "Enter content of file: ";
	getline(cin, content);
	ofstream f(filename + "." + type);
	f << content;
	f.close();
	branch->branchfiles.insert(filename, content);
	cout << "Successfully committed changes." << endl;
	writeInBranchFiles(id, filename + "." + type, branch->branchowner, branch->branchName);
}

bool RepositoryTree::iscommit()
{
	int c;
	cout << "Do you want to commit changes?" << endl;
	cout << "1 - Commit changes" << endl;
	cout << "2 - Do not commit changes" << endl;
	cin >> c;
	return (c == 1);
}
void RepositoryTree::writeInBranches(int id, BranchNode* branch)
{
	ifstream inFile("branches.csv");
	if (!inFile.is_open())
	{
		cerr << "Error: Unable to open branches.csv for reading." << endl;
		return;
	}

	vector<string> lines;
	string line;
	bool idFound = false;

	// Read each line from branches.csv and update the line if needed
	while (getline(inFile, line))
	{
		stringstream ss(line);
		string repoID;
		getline(ss, repoID, ',');

		// If repoID matches, append branch information
		if (convertStringToInt(repoID) == id)
		{
			line += "_" + branch->branchowner + "," + branch->branchName + "," +
				(branch->branchmode ? "public" : "private") + "," + branch->branchreadme->filename;
			idFound = true;
		}
		lines.push_back(line);
	}

	inFile.close();

	// If repoID not found, add a new line for it
	if (!idFound)
	{
		lines.push_back(to_string(id) + "_" + branch->branchowner + "," +
			branch->branchName + "," + (branch->branchmode ? "public" : "private") +
			"," + branch->branchreadme->filename);
	}

	// Rewrite the entire file with modified lines
	ofstream outFile("branches.csv", ios::trunc); // Open in truncation mode to clear existing content
	for (const auto& modifiedLine : lines)
	{
		outFile << modifiedLine << endl;
	}
	outFile.close();
}
void RepositoryTree::LoadfromBraches(RepositoryTree& tree)
{
	ifstream inFile("branches.csv");
	if (!inFile.is_open())
	{
		
		return;
	}

	string line;
	while (getline(inFile, line))
	{
		stringstream ss(line);
		string repoID, branchOwner, branchName, branchMode, branchReadme;

		getline(ss, repoID, ',');
		getline(ss, branchOwner, '_'); // Branch owner is separated by '_'
		getline(ss, branchName, ',');
		getline(ss, branchMode, ',');
		getline(ss, branchReadme, ',');

		// Convert branchMode to boolean
		bool mode = (branchMode == "public") ? true : false;

		// Create a new BranchNode
		BranchNode* branch = new BranchNode(branchOwner, branchName, mode, new FileNode(branchReadme, ""));

		// Insert the branch into the tree's branches
		tree.root->branches.insert(branch->branchowner, branch->branchName, branch->branchmode, branch->branchreadme);

	}

	inFile.close();
}
void RepositoryTree::loadFromForksCSV(RepositoryTree& tree) {
	ifstream forksFile("forks.csv");
	if (!forksFile.is_open()) {
		
		return;
	}

	string line;
	while (getline(forksFile, line)) {
		stringstream ss(line);
		string id, count;
		getline(ss, id, ',');
		getline(ss, count);

		// Convert id and count strings to integers
		int repoID = convertStringToInt(id);
		int fCount = convertStringToInt(count);

		// Update fork count for repository with repoID in the provided tree
		RepoNode* repo = tree.searchRepoByID(repoID, tree.root);
		if (repo != nullptr) {
			repo->forkcount = fCount;

		}
		else {

		}
	}


	forksFile.close();
}
void RepositoryTree::viewRepository(RepoNode* repo, string loggedinUser)
{

	if (repo->ownername == loggedinUser)
	{
		cout << "********************************REPOSITORY DETAILS************************************" << endl;
		cout << "Repository Owner Name:" << repo->ownername << endl;
		cout << "Repository ID: " << repo->repoID << endl;
		cout << "Repository Name: " << repo->RepoName << endl;
		cout << "Repository Fork Count: " << repo->forkcount << endl;
		cout << "Repository Branches: ";
		bool found = false;
		string BN;
		while (repo->branches.head != NULL)
		{
			cout << repo->branches.head->branchName << endl;
			BN = repo->branches.head->branchName;
			if (repo != nullptr) {
				BranchNode* currentBranch = repo->branches.head;
				while (currentBranch != nullptr) {
					if (currentBranch->branchName == BN) {
						cout << "Branch found: " << BN << endl;
						FileNode* currentFile = currentBranch->branchfiles.head;
						if (currentFile != nullptr) {
							cout << "Files in branch " << BN << " of repository ID " << repo->repoID << ":" << endl;
							while (currentFile != nullptr) {
								cout << currentFile->filename << endl;
								currentFile = currentFile->next;
							}
						}
						else {
							cout << "No files found in branch " << BN << endl; // Debug statement
						}
						break; // Exit the loop after finding the specified branch
					}
					currentBranch = currentBranch->next;
				}
				if (currentBranch == nullptr) {
					cout << "Branch " << BN << " not found in repository with ID " << repo->repoID << endl; // Debug statement
				}
			}
			else {
				cout << "Repository with ID " << repo->repoID << " not found." << endl;
			}
			repo->branches.head = repo->branches.head->next;
			found = true;
		}
		if (found == false)
		{
			cout << "This repo id have no branches" << endl;
		}
	}
	else if (repo->mode == false)
	{
		cout << "You cannot view private repository" << endl;
	}
	else {
		cout << "Repo id not found " << endl;
	}

}
