from sklearn.metrics import accuracy_score
from sklearn.pipeline import make_pipeline
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
import pandas as pd


df = pd.read_csv("spambase.data")
x = df.iloc[:,:-1]
y = df.iloc[:,-1]


x = StandardScaler().fit(x).transform(x)

# converted to pd dataframe
x = pd.DataFrame(x)

x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.3, random_state=42)

svm = make_pipeline(StandardScaler(), SVC(kernel="rbf", C = 2))

svm.fit(x_train, y_train)

y_predict = svm.predict(x_test)

acc = accuracy_score(y_test, y_predict)

print(acc)

# svm.score(x_test, y_test)

# print(df.head())

