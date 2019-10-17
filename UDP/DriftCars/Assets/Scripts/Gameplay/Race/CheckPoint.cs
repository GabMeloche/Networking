using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckPoint : MonoBehaviour
{
    [SerializeField]
    int iD;

    public int ID
    {
        get { return iD; }
        set { iD = value; }
    }

    public delegate void CarEnterDelegate(CheckPoint cp, Car car);
    public event CarEnterDelegate OnValidateCheckpoint;
	
	void OnTriggerEnter(Collider collider)
    {
        Car car = collider.GetComponent<Car>();

        if (car != null)
            OnValidateCheckpoint(this, car);
	}
}
