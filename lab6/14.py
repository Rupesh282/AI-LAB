from sklearn.metrics import accuracy_score
from sklearn.pipeline import make_pipeline
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
import pandas as pd
import sys

kernel_name = sys.argv[1]

print("kernel Name : " , kernel_name)

df = pd.read_csv("spambase.data")
x = df.iloc[:,:-1]
y = df.iloc[:,-1]


x = StandardScaler().fit(x).transform(x)

# converted to pd dataframe
x = pd.DataFrame(x)

x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.3, random_state=42)

svm = SVC(kernel=kernel_name, C = 4)

svm.fit(x_train, y_train)

y_train_predict = svm.predict(x_train)
y_test_predict = svm.predict(x_test)

print("train, test accuracies : ")
print(accuracy_score(y_train, y_train_predict), accuracy_score(y_test, y_test_predict))

# printing in file
# for i in range(1, 50) : 
#     if kernel_name == "poly" : 
#         svm = SVC(kernel=kernel_name, C = i, degree = 2)
#     else : 
#         svm = SVC(kernel=kernel_name, C = i)
#     svm.fit(x_train, y_train)
#     y_train_predict = svm.predict(x_train)
#     y_test_predict = svm.predict(x_test)
#     print(i, accuracy_score(y_train, y_train_predict), accuracy_score(y_test, y_test_predict))


# svm.score(x_test, y_test)

# print(df.head())

